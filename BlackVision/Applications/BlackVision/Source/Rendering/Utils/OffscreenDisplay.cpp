#include "OffscreenDisplay.h"

// #include "Engine/Graphics/Renderers/Renderer.h"

#include "Rendering/Utils/RenderTargetStackAllocator.h"


namespace bv {

// **************************
//
OffscreenDisplay::OffscreenDisplay    ( RenderTargetStackAllocator * rtAllocator, unsigned int numBufferedRenderTargets, bool hasVideoRenderTarget )
    : m_currentFrameRtIdx( 0 )
    , m_videoRenderTarget( nullptr )
{
    for( unsigned int i = 0; i < numBufferedRenderTargets; ++i )
    {
        m_bufferedFramesRenderTargets.push_back( rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_READ ) );
    }

    if( hasVideoRenderTarget )
    {
        m_videoRenderTarget = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_READ );
    }
}

// **************************
//
OffscreenDisplay::~OffscreenDisplay                         ()
{
}

// **************************
//
void            OffscreenDisplay::UpdateActiveRenderTargetIdx   ()
{
    m_currentFrameRtIdx = ( m_currentFrameRtIdx + 1 ) % TotalFrameRenderTargets();
}

// **************************
//
RenderTarget *  OffscreenDisplay::GetCurrentFrameRenderTarget       ()
{
    return m_bufferedFramesRenderTargets[ m_currentFrameRtIdx ];
}

// **************************
//
RenderTarget *  OffscreenDisplay::GetPreviousFrameRenderTarget      ()
{
    unsigned int idx = m_currentFrameRtIdx > 0 ? m_currentFrameRtIdx - 1 : TotalFrameRenderTargets() - 1;

    return m_bufferedFramesRenderTargets[ idx ];
}

// **************************
//
unsigned int    OffscreenDisplay::TotalFrameRenderTargets   () const
{
    return (unsigned int) m_bufferedFramesRenderTargets.size();
}

// **************************
//
RenderTarget *  OffscreenDisplay::GetVideoRenderTarget      ()
{
    return m_videoRenderTarget;
}

} //bv
