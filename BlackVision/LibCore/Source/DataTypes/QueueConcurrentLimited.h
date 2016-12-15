#pragma once

#include <cassert>
#include <queue>
#include <condition_variable>

#include "Threading/ScopedCriticalSection.h"
#include "CoreDEF.h"


namespace bv
{

//Based on https://www.cs.mtu.edu/~shene/NSF-3/e-Book/SEMA/TM-example-buffer.html
template< typename T >
class QueueConcurrentLimited
{
private:

    SizeType                            m_maxSize;

    mutable std::condition_variable     m_notFullCond;
    mutable std::condition_variable     m_notEmptyCond;
    mutable std::condition_variable     m_bufferLockCond;

    mutable std::mutex                  m_mutexNotFull;
    mutable std::mutex                  m_mutexBufferLock;
    mutable std::mutex                  m_mutexNotEmpty;

    std::queue< T >     m_queue;

public:

    explicit    QueueConcurrentLimited  ( SizeType maxSize );
                ~QueueConcurrentLimited ();
                QueueConcurrentLimited  ( const QueueConcurrentLimited< T > & );

    bool        IsEmpty                 () const;
    size_t      Size                    () const;
    bool        IsFull                  () const;

    bool        TryPush                 ( const T & val );
    bool        TryPush                 ( const T && val );

    void        WaitAndPush             ( const T & val );
    void        WaitAndPush             ( const T && val );

    bool        Front                   ( T & val );

    bool        TryPop                  ( T & val );
    void        WaitAndPop              ( T & val );

    void        Clear                   ();
};

// *************************************
//
template< typename T >
QueueConcurrentLimited< T >::QueueConcurrentLimited         ( SizeType maxSize )
    : m_maxSize( maxSize )
{}

// *************************************
//
template< typename T >
QueueConcurrentLimited< T >::~QueueConcurrentLimited        ()
{}

// *************************************
//
template< typename T >
QueueConcurrentLimited< T >::QueueConcurrentLimited         ( const QueueConcurrentLimited< T > & other )
{
    m_queue = other.m_queue;

    m_notFullCond = other.m_notFullCond;
    m_notEmptyCond = other.m_notEmptyCond;
    m_bufferLockCond = other.m_bufferLockCond;

    m_maxSize = other.m_maxSize;
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::IsEmpty            () const
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    auto ret = m_queue.empty();

    m_bufferLockCond.notify_one();

    return ret;
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::IsFull             () const
{
    return Size() >= m_maxSize;
}

// *************************************
//
template< typename T >
size_t      QueueConcurrentLimited< T >::Size               () const
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    auto s = m_queue.size();

    m_bufferLockCond.notify_one();

    return s;
}

// *************************************
//
template< typename T >
void        QueueConcurrentLimited< T >::WaitAndPush        ( const T & val )
{
    std::unique_lock< std::mutex > lock( m_mutexNotFull );
    m_notFullCond.wait( lock, [ = ] { return !IsFull(); } );

    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    m_queue.push( val );

    m_bufferLockCond.notufy_one();
    m_notEmptyCond.notify_one();
}

// ***********************
//
template< typename T >
void        QueueConcurrentLimited< T >::WaitAndPush        ( const T && val )
{
    std::unique_lock< std::mutex > lock( m_mutexNotFull );
    m_notFullCond.wait( lock, [ = ] { return !IsFull(); } );

    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    m_queue.push( val );

    m_bufferLockCond.notify_one();
    m_notEmptyCond.notify_one();
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::TryPush            ( const T & val )
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    if( Size() < m_maxSize )
    {
        m_queue.push( val );
        m_notEmptyCond.notify_one();
        m_bufferLockCond.notify_one();
        return true;
    }
    else
    {
        m_bufferLockCond.notify_one();
        return false;
    }
}

// ***********************
//
template< typename T >
bool        QueueConcurrentLimited< T >::TryPush            ( const T && val )
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    if( Size() < m_maxSize )
    {
        m_queue.push( val );
        m_notEmptyCond.notify_one();
        m_bufferLockCond.notify_one();
        return true;
    }
    else
    {
        m_bufferLockCond.notify_one();
        return false;
    }
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::Front              ( T & val )
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    if( m_queue.empty() )
    {
        m_bufferLockCond.notify_one();
        return false;
    }

    val = m_queue.front();

    m_bufferLockCond.notify_one();
    return true;
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::TryPop             ( T & val )
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );
    
    if( m_queue.empty() )
    {
        m_notEmptyCond.notify_one();
        m_bufferLockCond.notify_one();
        return false;
    }
    else
    {
        val = m_queue.front();
        m_queue.pop();

        m_notEmptyCond.notify_one();
        m_bufferLockCond.notify_one();

        return true;
    }
}

// *************************************
//
template< typename T >
void        QueueConcurrentLimited< T >::WaitAndPop         ( T & val )
{
    std::unique_lock< std::mutex > lock( m_mutexNotEmpty );
    m_notEmptyCond.wait( lock, [ = ] { return !IsEmpty(); } );

    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    val = m_queue.front();
    m_queue.pop();

    m_notFullCond.notify_one();
    m_bufferLockCond.notify_one();
}

// *************************************
//
template< typename T >
void        QueueConcurrentLimited< T >::Clear              ()
{
    std::unique_lock< std::mutex > lock1( m_mutexBufferLock );
    m_bufferLockCond.wait( lock1 );

    while( !m_queue.empty() )
    {
        m_queue.pop();
    }

    m_bufferLockCond.notify_one();
}

} //bv
