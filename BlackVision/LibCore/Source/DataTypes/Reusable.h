#pragma once

#include "CoreDEF.h"

#include <boost/circular_buffer.hpp>

#include <vector>



namespace bv
{



// ***********************
/// Class holds reusable elements in circular buffer. You can buffers and fill with new data
/// without need to memory realocations.
template< typename ElementType >
class Reusable
{
private:

    boost::circular_buffer< ElementType >      m_queue;

public:

    explicit        Reusable    ( const std::vector< ElementType > & reusables );

    ElementType     GetNext     ();
};



// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
template< typename ElementType >
inline          Reusable< ElementType >::Reusable       ( const std::vector< ElementType > & reusables )
    :   m_queue( reusables.size() )
{
    assert( reusables.size() > 0 );

    for( auto & element : reusables )
    {
        m_queue.push_back( element );
    }
}

// ***********************
//
template< typename ElementType >
inline ElementType      Reusable< ElementType >::GetNext        ()
{
    auto element = m_queue.front();
    
    m_queue.pop_front();
    m_queue.push_back( element );

    return element;
}


}	// bv



