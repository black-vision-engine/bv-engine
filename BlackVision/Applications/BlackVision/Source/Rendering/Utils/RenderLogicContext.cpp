#include "RenderLogicContext.h"


namespace bv {

// ***************************
//
RenderLogicContext::RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderLogic * renderLogic, DrawLogic * drawLogic )
    : m_renderer( renderer )
    , m_rtAllocator( rtStackAllocator )
    , m_renderLogic( renderLogic )
    , m_drawLogic( drawLogic )
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
DrawLogic *                     RenderLogicContext::GetDrawLogic            () const
{
    return m_drawLogic;
}

} // bv
