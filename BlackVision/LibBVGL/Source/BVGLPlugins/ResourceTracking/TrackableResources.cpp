#include "TrackableResources.h"

#include <cassert>
#include <sstream>
#include <iomanip>

#include "BVGLPlugins/ResourceTracking/ResourceTrackingDescriptors.h"


namespace bv {

// *****************************
//
template< typename ResourceDesc >
void            TrackableResources< ResourceDesc >::GenResources        ( GLsizei n, GLuint * resources )
{
    for( auto i = 0; i < n; ++i )
    {
        auto resource = resources[ i ];

        m_allocatedResources[ resource ] = ResourceDesc();
    }
}

// *****************************
//
template< typename ResourceDesc >
void            TrackableResources< ResourceDesc >::CreateResources     ( GLsizei n, GLuint * resources )
{
    GenResources( n, resources );
}

// *****************************
//
template< typename ResourceDesc >
void            TrackableResources< ResourceDesc >::BindResource        ( GLenum target, GLuint resource )
{
    m_boundResources[ target ] = resource;
}

// *****************************
//
template< typename ResourceDesc >
void            TrackableResources< ResourceDesc >::DeleteResources     ( GLsizei n, const GLuint * resources )
{
    for( auto i = 0; i < n; ++i )
    {
        auto resource = resources[ i ];

        m_allocatedResources.erase( resource );
    }    
}

// *****************************
//
template< typename ResourceDesc >
GLuint          TrackableResources< ResourceDesc >::GetBoundResourceID  ( GLenum target ) const
{
    assert( m_boundResources.find( target ) != m_boundResources.end() );
    
    return m_boundResources.find( target )->second;
}

// *****************************
//
template< typename ResourceDesc >
ResourceDesc &  TrackableResources< ResourceDesc >::GetBoundResource    ( GLenum target )
{
    return m_allocatedResources[ GetBoundResourceID( target ) ];
}

// *****************************
//
template< typename ResourceDesc >
std::string            TrackableResources< ResourceDesc >::ShortSummary ( const std::string & header ) const
{
    std::stringstream ss;

    if ( header != "" )
    {
        ss << header << std::endl;    
    }

    auto visitor = ResourceDesc::VisitorAggregate();

    for ( auto elt : 

    return ss.str();
}

// *****************************
//
template< typename ResourceDesc >
std::string             TrackableResources< ResourceDesc >::LongSummary ( const std::string & header ) const
{
    std::stringstream ss;

    if ( header != "" )
    {
        ss << header << std::endl;    
    }

    for( auto it = m_allocatedResources.begin(); it != m_allocatedResources.end(); ++it )
    {
        auto resID          = it->first;
        const auto & res    = it->second;
        auto target         = BoundTo( resID );
    
        ss << std::setfill( ' ' ) << std::setw( 3 ) << resID << " ";
        ss << res.Summary();

        if( target > 0 )
        {
            ss << " BND: ";
            ss << res.TargetStr( target );
        }
    
        ss << std::endl;
    }

    return ss.str();
}

// *****************************
//
template< typename ResourceDesc >
GLenum                  TrackableResources< ResourceDesc >::BoundTo ( GLuint resID ) const
{
    for( auto & elt : m_boundResources )
    {
        if( elt->second == resID )
            return elt->first;
    }

    return 0;
}

// Explicitely instantiate all resource types here
template class TrackableResources< BufferDesc >;
template class TrackableResources< TextureDesc >;
template class TrackableResources< RenderbufferDesc >;
template class TrackableResources< FramebufferDesc >;
template class TrackableResources< VertexArrayDesc >;

} // bv
