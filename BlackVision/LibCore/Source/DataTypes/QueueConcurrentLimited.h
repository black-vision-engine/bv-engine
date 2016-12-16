#pragma once

#include <cassert>
#include <queue>
#include <condition_variable>

#include "Threading/ScopedCriticalSection.h"
#include "Semaphore.h"



namespace bv
{

//Based on https://www.cs.mtu.edu/~shene/NSF-3/e-Book/SEMA/TM-example-buffer.html
template< typename T >
class QueueConcurrentLimited
{
private:

    Semaphore       m_notEmpty;
    Semaphore       m_notFull;
    std::mutex      m_bufferLock;

    std::queue< T >     m_queue;
    SizeType            m_maxSize;

public:

    explicit    QueueConcurrentLimited  ( SizeType maxSize );
                ~QueueConcurrentLimited ();

    bool        TryPush                 ( const T & val );
    bool        TryPush                 ( const T && val );

    void        WaitAndPush             ( const T & val );
    void        WaitAndPush             ( const T && val );

    bool        Front                   ( T & val );

    bool        TryPop                  ( T & val );
    void        WaitAndPop              ( T & val );

    /*void        Clear                   ();*/

    bool        IsEmpty                 () const;
    size_t      Size                    () const;
};

// *************************************
//
template< typename T >
QueueConcurrentLimited< T >::QueueConcurrentLimited         ( SizeType maxSize )
    :   m_notFull( maxSize )
    ,   m_notEmpty( 0 )
    ,   m_maxSize( maxSize )
{
    // Can cause deadlock.
    assert( maxSize != 0 );
}

// *************************************
//
template< typename T >
QueueConcurrentLimited< T >::~QueueConcurrentLimited        ()
{}


// *************************************
//  Note: external use only
template< typename T >
bool        QueueConcurrentLimited< T >::IsEmpty            () const
{
    std::unique_lock< std::mutex > lock( m_mutexBufferLock );
    return m_queue.empty();
}


// *************************************
//  Note: external use only
template< typename T >
size_t      QueueConcurrentLimited< T >::Size               () const
{
    std::unique_lock< std::mutex > lock( m_mutexBufferLock );
    return m_queue.size();
}


// *************************************
//
template< typename T >
void        QueueConcurrentLimited< T >::WaitAndPush        ( const T & val )
{
    m_notFull.Down();

    m_bufferLock.lock();
    m_queue.push( val );
    m_bufferLock.unlock();

    m_notEmpty.Up();
}

// ***********************
//
template< typename T >
void        QueueConcurrentLimited< T >::WaitAndPush        ( const T && val )
{
    m_notFull.Down();

    m_bufferLock.lock();
    m_queue.push( std::move( val ) );
    m_bufferLock.unlock();

    m_notEmpty.Up();
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::TryPush            ( const T & val )
{
    if( m_notFull.TryDown() )
    {
        m_bufferLock.lock();
        m_queue.push( val );
        m_bufferLock.unlock();

        m_notEmpty.Up();

        return true;
    }
    return false;
}

// ***********************
//
template< typename T >
bool        QueueConcurrentLimited< T >::TryPush            ( const T && val )
{
    if( m_notFull.TryDown() )
    {
        m_bufferLock.lock();
        m_queue.push( std::move( val ) );
        m_bufferLock.unlock();

        m_notEmpty.Up();

        return true;
    }
    return false;
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::Front              ( T & val )
{
    std::unique_lock< std::mutex > lock( m_bufferLock );

    if( !m_queue.empty() )
    {
        val = m_queue.front();
        return true;
    }
    return false;
}

// *************************************
//
template< typename T >
bool        QueueConcurrentLimited< T >::TryPop             ( T & val )
{
    if( m_notEmpty.TryDown() )
    {
        m_bufferLock.lock();

        val = m_queue.front();
        m_queue.pop();

        m_bufferLock.unlock();

        m_notFull.Up();
        return true;
    }
    return false;
}

// *************************************
//
template< typename T >
void        QueueConcurrentLimited< T >::WaitAndPop         ( T & val )
{
    m_notEmpty.Down();

    m_bufferLock.lock();

    val = m_queue.front();
    m_queue.pop();

    m_bufferLock.unlock();

    m_notFull.Up();
}

// *************************************
//
//template< typename T >
//void        QueueConcurrentLimited< T >::Clear              ()
//{
//    while( TryPop() );
//}

} //bv
