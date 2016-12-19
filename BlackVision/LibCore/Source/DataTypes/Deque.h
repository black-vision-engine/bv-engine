#pragma once

#include <deque>


namespace bv
{

template< typename T >
class Deque
{
private:

    std::deque< T >     m_queue;

public:

    explicit    Deque       ();
                ~Deque      ();

    bool        IsEmpty     () const;
    size_t      Size        () const;

    const T &   Back        () const;
    T &         Back        ();

    const T &   Front       () const;
    T &         Front       ();

    void        PopFront    ();
    void        PopBack     ();

    void        PushFront   ( const T & val );
    void        PushBack    ( const T & val );

    void        Swap        ( Deque & other );

    void        Clear       ();

};

// *************************************
//
template< typename T >
Deque< T >::Deque       ()
{
}

// *************************************
//
template< typename T >
Deque< T >::~Deque       ()
{
}

// *************************************
//
template< typename T >
bool        Deque< T >::IsEmpty     () const
{
    return m_queue.empty();
}

// *************************************
//
template< typename T >
size_t      Deque< T >::Size        () const
{
    return m_queue.size();
}

// *************************************
//
template< typename T >
const T &   Deque< T >::Back        () const
{
    return m_queue.back();
}

// *************************************
//
template< typename T >
T &         Deque< T >::Back        ()
{
    return m_queue.back();
}

// *************************************
//
template< typename T >
const T &   Deque< T >::Front       () const
{
    return m_queue.front();
}

// *************************************
//
template< typename T >
T &         Deque< T >::Front       ()
{
    return m_queue.front();
}

// *************************************
//
template< typename T >
void        Deque< T >::PopFront    ()
{
    m_queue.pop_front();
}

// *************************************
//
template< typename T >
void        Deque< T >::PopBack     ()
{
    m_queue.pop_back();
}

// *************************************
//
template< typename T >
void        Deque< T >::PushFront   ( const T & val )
{
    m_queue.push_front( val );
}

// *************************************
//
template< typename T >
void        Deque< T >::PushBack   ( const T & val )
{
    m_queue.push_back( val );
}

// *************************************
//
template< typename T >
void        Deque< T >::Swap        ( Deque & other )
{
    m_queue.swap( other.m_queue );
}

// *************************************
//
template< typename T >
void        Deque< T >::Clear       ()
{
    while( !m_queue.empty() )
    {
        //auto val = m_queue.front();
        m_queue.pop_back();
    }
}

}
