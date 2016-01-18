#include "RenderLogicContext.h"


namespace bv {

// ***************************
//
RenderLogicContext::RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderLogic * renderLogic )
    : m_renderer( renderer )
    , m_rtAllocator( rtStackAllocator )
    , m_renderLogic( renderLogic )
{
}

// ***************************
//
RenderLogicContext::~RenderLogicContext ()
{
}

// ***************************
//
Renderer *                      RenderLogicContext::GetRenderer             () const
{
    return m_renderer;
}

// ***************************
//
RenderTargetStackAllocator *    RenderLogicContext::GetRenderTargetAllocator() const
{
    return m_rtAllocator;
}

// ***************************
//
RenderLogic *                   RenderLogicContext::GetRenderLogic          () const
{
    return m_renderLogic;
}

// ***************************
//
RenderTarget *                  RenderLogicContext::GetBoundRenderTarget    () const
{
    return m_boundRenderTarget;
}

// ***************************
//
void                            RenderLogicContext::SetBoundRenderTarget    ( RenderTarget * target )
{
    m_boundRenderTarget = target;
}

} // bv
