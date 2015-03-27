#include "TrackableResources.h"

#include <cassert>

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
void            TrackableResources< ResourceDesc >::PrintShortSummary   () const
{
}

// *****************************
//
template< typename ResourceDesc >
void            TrackableResources< ResourceDesc >::PrintLongSummary    () const
{
}


// Explicitely instantiate all resource types here
template class TrackableResources< BufferDesc >;
template class TrackableResources< TextureDesc >;
template class TrackableResources< RenderbufferDesc >;
template class TrackableResources< FramebufferDesc >;
template class TrackableResources< VertexArrayDesc >;

} // bv
