#include "OffscreenDisplay.h"

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"


namespace {

    const unsigned int GNumVideoRenderTaregets = 1;

} // anonymous

namespace bv {

// **************************
//
OffscreenDisplay::OffscreenDisplay    ( RenderTargetStackAllocator * rtAllocator, unsigned int numBufferedRenderTargets, bool hasVideoRenderTarget )
    : m_currentFrameRtIdx( 0 )
    , m_currentVideoRtIdx( 0 )
{
    if( hasVideoRenderTarget )
    {
        for( unsigned int i = 0; i < GNumVideoRenderTaregets; ++i )
        {
            m_videoRenderTargets.push_back( rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_READ ) );
        }
    }

    for( unsigned int i = 0; i < numBufferedRenderTargets; ++i )
    {
        m_bufferedFramesRenderTargets.push_back( rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_READ ) );
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
void            OffscreenDisplay::UpdateVideoRenderTargetIdx    ()
{
    m_currentVideoRtIdx = ( m_currentVideoRtIdx + 1 ) % GNumVideoRenderTaregets;
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
unsigned int    OffscreenDisplay::TotalVideoRenderTargets     () const
{
    return GNumVideoRenderTaregets;
}

// **************************
//
RenderTarget *  OffscreenDisplay::GetVideoRenderTarget      ()
{
    return m_videoRenderTargets[ m_currentVideoRtIdx ];
}

} //bv
