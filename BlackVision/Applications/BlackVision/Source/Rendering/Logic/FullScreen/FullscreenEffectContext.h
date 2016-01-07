#pragma once


namespace bv {

class Renderer;
class RenderTarget;
class RenderTargetStackAllocator;

class FullscreenEffectContext
{
private:

    Renderer *                      m_renderer;
    RenderTarget *                  m_outputRenderTarget;
    RenderTargetStackAllocator *    m_rtAllocator;

public:
                                    FullscreenEffectContext     ( Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator );
                                    ~FullscreenEffectContext    ();

    Renderer *                      GetRenderer                 ();
    RenderTarget *                  GetOutputRenderTarget       ();
    RenderTargetStackAllocator *    GetRenderTargetAllocator    ();

    void                            SetOutputRenderTarget       ( RenderTarget * rt );

};

} //bv
