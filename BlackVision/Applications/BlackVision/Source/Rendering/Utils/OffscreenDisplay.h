#pragma once

#include <vector>


namespace bv {

class RenderTargetStackAllocator;
class RenderTarget;


class OffscreenDisplay
{
private:

    std::vector< RenderTarget * >   m_bufferedFramesRenderTargets;
    std::vector< RenderTarget * >   m_videoRenderTargets;

    unsigned int                    m_currentFrameRtIdx;
    unsigned int                    m_currentVideoRtIdx;

public:

                    OffscreenDisplay            ( RenderTargetStackAllocator * rtAllocator, unsigned int numBufferedRenderTargets, bool hasVideoRenderTarget );
                    ~OffscreenDisplay           ();

    //Frame rendering
    void            UpdateActiveRenderTargetIdx ();
    void            UpdateVideoRenderTargetIdx  ();

    RenderTarget *  GetCurrentFrameRenderTarget ();
    RenderTarget *  GetPreviousFrameRenderTarget();

    unsigned int    TotalFrameRenderTargets     () const;
    unsigned int    TotalVideoRenderTargets     () const;

    //Video display
    RenderTarget *  GetVideoRenderTarget        ();

};

} //bv
