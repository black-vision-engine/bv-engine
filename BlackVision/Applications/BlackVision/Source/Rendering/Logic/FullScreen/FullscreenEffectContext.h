#pragma once

#include <vector>


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
    std::vector< RenderTarget * >   m_inputRenderTargets;

    unsigned int                    m_startIndex;

public:
                                    FullscreenEffectContext     ( Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator, unsigned int startIndex = 0 );
                                    ~FullscreenEffectContext    ();

    Renderer *                      GetRenderer                 ();
    RenderTarget *                  GetOutputRenderTarget       ();
    RenderTargetStackAllocator *    GetRenderTargetAllocator    ();

    void                            SetOutputRenderTarget       ( RenderTarget * rt );

    void                            AppendInputRenderTarget     ( RenderTarget * rt );
    std::vector< RenderTarget * > & AccessInputRenderTargets    ();

    void                            SetFirstRenderTargetIndex   ( unsigned int startIndex );
    unsigned int                    GetFirstRenderTargetIndex   () const;

};

} //bv
