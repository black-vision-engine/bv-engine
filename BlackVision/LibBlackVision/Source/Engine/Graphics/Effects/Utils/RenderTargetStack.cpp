#include "RenderTargetStack.h"

#include <cassert>

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Utils/RenderTargetFactory.h"


namespace bv {


// *********************************
//
RenderTargetStack::RenderTargetStack           ( unsigned int width, unsigned int height, TextureFormat fmt )
    : m_isTopRTEnabled( false )
    , m_topActiveRenderTargetIdx( -1 )
    , m_renderTargetDescriptor( width, height, fmt )
{
}

// *********************************
//
RenderTargetStack::~RenderTargetStack          ()
{
}

// *********************************
//
void    RenderTargetStack::AllocateNewRenderTarget              ( Renderer * renderer, RenderTarget::RTSemantic semantic )
{
    if( m_isTopRTEnabled )
    {
        DisableTopRenderTarget( renderer );
    }

    m_topActiveRenderTargetIdx++;

    if( m_topActiveRenderTargetIdx >= (int) m_allocatedRenderTargets.size() )
    {
        auto rt = RenderTargetFactory::CreateRenderTarget( semantic, m_renderTargetDescriptor.width, m_renderTargetDescriptor.height, m_renderTargetDescriptor.fmt );

        m_allocatedRenderTargets.push_back( rt );
    }
    else
    {
        //FIXME: we assume that usage patterns are exactly the same between consecutive frames which may not hold in a more general case
        //FIXME: in such case additional means of RT management should be used (first of all m_topActiveRenderTargetIdx must be removed from GPU - the hard part
        //FIXME: and the it has to be removed from m_allocatedRenderTargets and replaced with the new one created according to required semantics).
        assert( GetRenderTargetAt( m_topActiveRenderTargetIdx )->Semantic() == semantic );
    }

    assert( m_topActiveRenderTargetIdx < (int) m_allocatedRenderTargets.size() );
}

// *********************************
//
void    RenderTargetStack::EnableTopRenderTarget                ( Renderer * renderer )
{
    assert( m_topActiveRenderTargetIdx >= 0 );

    if( !m_isTopRTEnabled )
    {
        renderer->Enable( GetRenderTargetAt( -1 ) );

        m_isTopRTEnabled = true;
    }
}

// *********************************
//
void    RenderTargetStack::DiscardCurrentRenderTarget           ( Renderer * renderer )
{
    assert( m_topActiveRenderTargetIdx >= 0 );

    if( m_isTopRTEnabled )
    {
        DisableTopRenderTarget( renderer );
    }

    m_topActiveRenderTargetIdx--;
}

// *********************************
//
void    RenderTargetStack::DisableTopRenderTarget               ( Renderer * renderer )
{
    if( m_isTopRTEnabled )
    {
        renderer->Disable( GetRenderTargetAt( -1 ) );

        m_isTopRTEnabled = false;
    }
}

// *********************************
//
RenderTarget *  RenderTargetStack::GetRenderTargetAt            ( int i ) const
{
    if( i < 0 )
    {
        i = m_topActiveRenderTargetIdx + i + 1;
    }

    if( i < 0 || i > m_topActiveRenderTargetIdx )
    {
        assert( false );

        return nullptr;
    }

    return m_allocatedRenderTargets[ i ];
}

// *********************************
//
unsigned int    RenderTargetStack::TotalActiveRenderTargets    () const
{
    return m_topActiveRenderTargetIdx + 1;
}

// *********************************
//
unsigned int    RenderTargetStack::TotalAllocatedRenderTargets () const
{
    return (unsigned int) m_allocatedRenderTargets.size();
}

} //bv
