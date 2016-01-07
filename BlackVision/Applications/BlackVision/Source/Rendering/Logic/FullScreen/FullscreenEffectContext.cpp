#include "FullscreenEffectContext.h"


namespace bv {

// **************************
//
FullscreenEffectContext::FullscreenEffectContext            ( Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator )
    : m_renderer( renderer )
    , m_outputRenderTarget( nullptr )
    , m_rtAllocator( allocator )
{
    SetOutputRenderTarget( outputRenderTarget );
}

// **************************
//
FullscreenEffectContext::~FullscreenEffectContext           ()
{
}

// **************************
//
Renderer *                      FullscreenEffectContext::GetRenderer                 ()
{
    return m_renderer;
}

// **************************
//
RenderTarget *                  FullscreenEffectContext::GetOutputRenderTarget       ()
{
    return m_outputRenderTarget;
}

// **************************
//
RenderTargetStackAllocator *    FullscreenEffectContext::GetRenderTargetAllocator    ()
{
    return m_rtAllocator;
}

// **************************
//
void                            FullscreenEffectContext::SetOutputRenderTarget       ( RenderTarget * rt )
{
    m_outputRenderTarget = rt;
}

} //bv