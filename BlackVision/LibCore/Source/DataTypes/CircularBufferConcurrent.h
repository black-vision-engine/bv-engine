#pragma once

#include <mutex>
#include <condition_variable>
#include <boost\circular_buffer.hpp>


namespace bv {

//Based on implementation rom "C++ Concurrency in Action - Practical Multithreading" by Anthony Williams
//See: www.justsoftwaresolutions.co.uk
template< typename T, size_t N >
class CircularBufferConcurrent
{
private:

    boost::circular_buffer< T >	        m_queue;
    mutable std::mutex					m_mutex;
    std::condition_variable				m_condVarPush;
    std::condition_variable				m_condVarPop;

public:

    explicit    CircularBufferConcurrent    ();
                ~CircularBufferConcurrent   ();

    bool        IsEmpty             () const;
    size_t      Size                () const;

    void        Push                ( const T & val );
    T           Pop                 ();
    bool        TryPop              ( T & val );

    T           Front               ();

    void        Clear               ();

};

// *************************************
//
template< typename T, size_t N >
CircularBufferConcurrent< T, N >::CircularBufferConcurrent()
    : m_queue( N )
{}

// *************************************
//
template< typename T, size_t N >
CircularBufferConcurrent< T, N >::~CircularBufferConcurrent()
{}

// *************************************
//
template< typename T, size_t N >
bool        CircularBufferConcurrent< T, N >::IsEmpty             () const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    return m_queue.empty();
}

// *************************************
//
template< typename T, size_t N >
size_t      CircularBufferConcurrent< T, N >::Size                () const
{
    std::unique_lock< std::mutex > lock( m_mutex );
    return m_queue.size();
}

// *************************************
//
template< typename T, size_t N >
void        CircularBufferConcurrent< T, N >::Push        ( const T & val )
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_condVarPop.wait( lock, [ = ] { return !m_queue.full(); } );
    m_queue.push_back( val );
    m_condVarPush.notify_one();
}

// *************************************
//
template< typename T, size_t N >
T           CircularBufferConcurrent< T, N >::Front       ()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_condVarPush.wait( lock, [ = ] { return !m_queue.empty(); } );
    return m_queue.front();
}

// *************************************
//
template< typename T, size_t N >
T           CircularBufferConcurrent< T, N >::Pop      ()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_condVarPush.wait( lock, [ = ] { return !m_queue.empty(); } );
    auto val = m_queue.front();
    m_queue.pop_front();
    m_condVarPop.notify_one();
    return val;
}

// ***********************
//
template< typename T, size_t N >
inline bool     CircularBufferConcurrent< T, N >::TryPop        ( T & val )
{
    std::unique_lock< std::mutex > lock( m_mutex );

    if( m_queue.empty() )
        return false;

    val = m_queue.front();
    m_queue.pop_front();

    m_condVarPop.notify_one();

    return true;
}


//
//// *************************************
////
//template< typename T, size_t N >
//void        QueueConcurrent< T, N >::WaitAndPop    ( T & val )
//{
//    m_conditionVariable.wait();
//
//    val = m_queue.front();
//    m_queue.pop();
//}

// *************************************
//
template< typename T, size_t N >
void        CircularBufferConcurrent< T, N >::Clear         ()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    while( !m_queue.empty() )
    {
        m_queue.pop_back();
        m_condVarPop.notify_one();
    }
}

} //bv
