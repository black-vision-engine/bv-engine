#pragma once

#include <vector>


namespace bv {

class RenderTargetStackAllocator;
class RenderTarget;


class OffscreenDisplay
{
private:

    std::vector< RenderTarget * >   m_bufferedFramesRenderTargets;
    RenderTarget *                  m_videoRenderTarget;

    unsigned int                    m_currentFrameRtIdx;

public:

                    OffscreenDisplay            ( RenderTargetStackAllocator * rtAllocator, unsigned int numBufferedRenderTargets, bool hasVideoRenderTarget );
                    ~OffscreenDisplay           ();

    //Frame rendering
    void            UpdateActiveRenderTargetIdx ();

    RenderTarget *  GetCurrentFrameRenderTarget ();
    RenderTarget *  GetPreviousFrameRenderTarget();

    unsigned int    TotalFrameRenderTargets     () const;

    //Video display
    RenderTarget *  GetVideoRenderTarget        ();

};

} //bv
