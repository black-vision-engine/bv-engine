#pragma once

#include <cassert>
#include <queue>

#include "System/Threading/ScopedCriticalSection.h"


namespace bv
{

//Based on implementation rom "C++ Concurrency in Action - Practical Multithreading" by Anthony Williams
//See: www.justsoftwaresolutions.co.uk
template< typename T >
class QueueConcurrent
{
private:

    CriticalSection     m_criticalSection;

    std::queue< T >     m_queue;
    HANDLE              m_dataPushed;

public:

    explicit    QueueConcurrent     ();
                ~QueueConcurrent    ();

    bool        IsEmpty             () const;
    size_t      Size                () const;

    void        Push                ( const T & val );

    bool        TryPop              ( T & val );
    void        WaitAndPop          ( T & val );

    template< void (*deinitializer)( T ) >
    void        Clear               ()
    {
        ScopedCriticalSection lock( m_criticalSection );

        while( !m_queue.empty() )
        {
            auto val = m_queue.front();
            m_queue.pop();

            deinitializer( val );
        }
    }

};

// *************************************
//
template< typename T >
QueueConcurrent< T >::QueueConcurrent     ()
{
    m_dataPushed = CreateEvent( NULL, TRUE, FALSE, NULL );
}

// *************************************
//
template< typename T >
QueueConcurrent< T >::~QueueConcurrent    ()
{
    CloseHandle( m_dataPushed );
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
    {
        ScopedCriticalSection lock( m_criticalSection );

        m_queue.push( val );
    }

    PulseEvent( m_dataPushed );
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
void        QueueConcurrent< T >::WaitAndPop  ( T & val )
{
    ScopedCriticalSection lock( m_criticalSection );

    while( m_queue.empty() )
    {
        WaitForSingleObject( m_dataPushed );
    }

    val = m_queue.front();
    m_queue.pop();
}

} //bv
