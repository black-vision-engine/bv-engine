#pragma once

#include <vector>


namespace bv {

class RenderTargetStackAllocator;
class RenderTarget;
class Renderer;


class OffscreenDisplay
{
private:

    std::vector< RenderTarget * >   m_renderTargets;

    unsigned int                    m_activeRtIdx;

public:

                    OffscreenDisplay            ( RenderTargetStackAllocator * rtAllocator, bool useTwoRenderTargets );
                    ~OffscreenDisplay           ();

    void            EnableActiveRenderTarget    ( Renderer * renderer );
    void            DisableActiveRenderTarget   ( Renderer * renderer );

    void            UpdateActiveRenderTargetIdx ();

    RenderTarget *  GetActiveRenderTarget       ();
    RenderTarget *  GetPreviousRenderTarget     ();

    unsigned int    TotalRenderTargets          () const;

};

} //bv
