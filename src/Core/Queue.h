#pragma once

#include <queue>


namespace bv
{

template< typename T >
class Queue
{
private:

    std::queue< T >     m_queue;

public:

    explicit    Queue       ();
                ~Queue      ();

    bool        IsEmpty     () const;
    size_t      Size        () const;

    const T &   Back        () const;
    T &         Back        ();

    const T &   Front       () const;
    T &         Front       ();

    void        Pop         ();
    void        Push        ( const T & val );

    void        Swap        ( Queue & other );

};

// *************************************
//
template< typename T >
Queue< T >::Queue       ()
{
}

// *************************************
//
template< typename T >
Queue< T >::~Queue       ()
{
}

// *************************************
//
template< typename T >
bool        Queue< T >::IsEmpty     () const
{
    return m_queue.empty();
}

// *************************************
//
template< typename T >
size_t      Queue< T >::Size        () const
{
    return m_queue.size();
}

// *************************************
//
template< typename T >
const T &   Queue< T >::Back        () const
{
    return m_queue.back();
}

// *************************************
//
template< typename T >
T &         Queue< T >::Back        ()
{
    return m_queue.back();
}

// *************************************
//
template< typename T >
const T &   Queue< T >::Front       () const
{
    return m_queue.front();
}

// *************************************
//
template< typename T >
T &         Queue< T >::Front       ()
{
    return m_queue.front();
}

// *************************************
//
template< typename T >
void        Queue< T >::Pop         ()
{
    m_queue.pop();
}

// *************************************
//
template< typename T >
void        Queue< T >::Push        ( const T & val )
{
    m_queue.push( val );
}

// *************************************
//
template< typename T >
void        Queue< T >::Swap        ( Queue & other )
{
    m_queue.swap( other.m_queue );
}

}
