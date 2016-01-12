#include "FullscreenEffectContext.h"


namespace bv {

// **************************
//
FullscreenEffectContext::FullscreenEffectContext            ( Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator, unsigned int startIndex )
    : m_renderer( renderer )
    , m_inputRenderTargets( nullptr )
    , m_outputRenderTarget( nullptr )
    , m_rtAllocator( allocator )
    , m_startIndex( startIndex )
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

// **************************
//
void                            FullscreenEffectContext::SetInputRenderTargets       ( const std::vector< RenderTarget * > * renderTargets )
{
    m_inputRenderTargets = renderTargets;
}

// **************************
//
const std::vector< RenderTarget * > * FullscreenEffectContext::AccessInputRenderTargets () const
{
    return m_inputRenderTargets;
}

// **************************
//
void                            FullscreenEffectContext::SetFirstRenderTargetIndex   ( unsigned int startIndex )
{
    m_startIndex = startIndex;
}

// **************************
//
unsigned int                    FullscreenEffectContext::GetFirstRenderTargetIndex   () const
{
    return m_startIndex;
}

} //bv