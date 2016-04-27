#include "stdafx.h"

#include "RenderLogicContext.h"


namespace bv {

// ***************************
//
RenderLogicContext::RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderQueueStackAllocator * rqStackAllocator, RenderLogic * renderLogic )
    : m_renderer( renderer )
    , m_rtAllocator( rtStackAllocator )
    , m_renderLogic( renderLogic )
    , m_renderQueueAllocator( rqStackAllocator )
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

// ***********************
//
RenderQueueStackAllocator *     RenderLogicContext::GetRenderQueueAllocator () const
{
    return m_renderQueueAllocator;
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
