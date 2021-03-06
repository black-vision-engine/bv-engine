#pragma once

#include <cassert>
#include <queue>
#include <condition_variable>

#include "Threading/ScopedCriticalSection.h"


namespace bv {

//Based on implementation rom "C++ Concurrency in Action - Practical Multithreading" by Anthony Williams
//See: www.justsoftwaresolutions.co.uk
template< typename T >
class QueueConcurrent
{
private:

    mutable std::condition_variable     m_conditionVariable;
    mutable std::mutex					m_mutex;

    mutable CriticalSection             m_criticalSection;

    std::queue< T >     m_queue;

public:

    explicit    QueueConcurrent     ();
                ~QueueConcurrent    ();
                QueueConcurrent     ( const QueueConcurrent< T > & );

    bool        IsEmpty             () const;
    size_t      Size                () const;

    void        Push                ( const T & val );
    void        Push                ( const T && val );

    bool        Front               ( T & val );

    bool        TryPop              ( T & val );
    void        WaitAndPop          ( T & val );

    void        Clear               ();

};

// *************************************
//
template< typename T >
QueueConcurrent< T >::QueueConcurrent     ()
{}

// *************************************
//
template< typename T >
QueueConcurrent< T >::~QueueConcurrent    ()
{}

// *************************************
//
template< typename T >
QueueConcurrent< T >::QueueConcurrent     ( const QueueConcurrent< T > & other )
{
    m_queue = other.m_queue;
    m_conditionVariable = other.m_conditionVariable;
    m_criticalSection = other.m_criticalSection;
}

// *************************************
//
template< typename T >
bool        QueueConcurrent< T >::IsEmpty             () const
{
    ScopedCriticalSection lock( m_criticalSection );

    return m_queue.empty();
}

// *************************************
//
template< typename T >
size_t      QueueConcurrent< T >::Size                () const
{
    ScopedCriticalSection lock( m_criticalSection );

    return m_queue.size();
}

// *************************************
//
template< typename T >
void        QueueConcurrent< T >::Push        ( const T & val )
{
    ScopedCriticalSection lock( m_criticalSection );

    m_queue.push( val );

    m_conditionVariable.notify_one();
}

// ***********************
//
template< typename T >
void        QueueConcurrent< T >::Push                ( const T && val )
{
    ScopedCriticalSection lock( m_criticalSection );
    
    m_queue.push( val );

    m_conditionVariable.notify_one();
}

// *************************************
//
template< typename T >
bool        QueueConcurrent< T >::Front       ( T & val )
{
    ScopedCriticalSection lock( m_criticalSection );

    if( m_queue.empty() )
    {
        return false;
    }

    val = m_queue.front();

    return true;
}

// *************************************
//
template< typename T >
bool        QueueConcurrent< T >::TryPop      ( T & val )
{
    ScopedCriticalSection lock( m_criticalSection );
    
    if( m_queue.empty() )
    {
        return false;
    }

    val = m_queue.front();
    m_queue.pop();

    return true;
}

// *************************************
//
template< typename T >
void        QueueConcurrent< T >::WaitAndPop    ( T & val )
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_conditionVariable.wait( lock, [ = ] { return !m_queue.empty(); } );

    val = m_queue.front();
    m_queue.pop();
}

// *************************************
//
template< typename T >
void        QueueConcurrent< T >::Clear         ()
{
    ScopedCriticalSection lock( m_criticalSection );

    while( !m_queue.empty() )
    {
        //auto val = m_queue.front();
        m_queue.pop();
    }
}

} //bv
