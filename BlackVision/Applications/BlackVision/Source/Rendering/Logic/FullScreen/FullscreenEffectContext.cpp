#include "FullscreenEffectContext.h"


namespace bv {

// **************************
//
FullscreenEffectContext::FullscreenEffectContext            ( Renderer * renderer, RenderTargetStackAllocator * allocator )
    : m_renderer( renderer )
    , m_rtAllocator( allocator )
{
}

// **************************
//
FullscreenEffectContext::~FullscreenEffectContext           ()
{
}

// **************************
//
FullscreenEffectData &          FullscreenEffectContext::AccessData                  ()
{
    return m_data;
}

// **************************
//
Renderer *                      FullscreenEffectContext::GetRenderer                 ()
{
    return m_renderer;
}

// **************************
//
RenderTargetStackAllocator *    FullscreenEffectContext::GetRenderTargetAllocator    ()
{
    return m_rtAllocator;
}

} //bv