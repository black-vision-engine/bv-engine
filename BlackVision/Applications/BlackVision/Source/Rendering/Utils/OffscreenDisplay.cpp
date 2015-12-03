#include "OffscreenDisplay.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Rendering/Utils/RenderTargetStackAllocator.h"


namespace bv {

// **************************
//
OffscreenDisplay::OffscreenDisplay    ( RenderTargetStackAllocator * rtAllocator, bool useTwoRenderTargets )
{
    m_renderTargets.push_back( rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_READ ) );

    if( useTwoRenderTargets )
    {
        m_renderTargets.push_back( rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_READ ) );
    }

    m_activeRtIdx = 0;
}

// **************************
//
void            OffscreenDisplay::EnableActiveRenderTarget      ( Renderer * renderer )
{
    renderer->Enable( GetActiveRenderTarget() );
}

// **************************
//
void            OffscreenDisplay::DisableActiveRenderTarget     ( Renderer * renderer )
{
    renderer->Disable( GetActiveRenderTarget() );
}

// **************************
//
void            OffscreenDisplay::UpdateActiveRenderTargetIdx   ()
{
    m_activeRtIdx = ( m_activeRtIdx + 1 ) % TotalRenderTargets  ();
}

// **************************
//
RenderTarget *  OffscreenDisplay::GetActiveRenderTarget     ()
{
    return m_renderTargets[ m_activeRtIdx ];
}

// **************************
//
RenderTarget *  OffscreenDisplay::GetPreviousRenderTarget   ()
{
    return m_renderTargets[ ( m_activeRtIdx + 1 ) % TotalRenderTargets() ];
}

// **************************
//
unsigned int    OffscreenDisplay::TotalRenderTargets        () const
{
    return (unsigned int) m_renderTargets.size();
}

// **************************
//
OffscreenDisplay::~OffscreenDisplay                         ()
{
}

} //bv
