#include "stdafx.h"

#include "RenderLogicContext.h"



#include "Memory/MemoryLeaks.h"


namespace bv {

// ***************************
//
RenderLogicContext::RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderQueueStackAllocator * rqStackAllocator, RenderLogic * renderLogic, audio::AudioRenderer * audioRenderer )
    : m_renderer( renderer )
    , m_rtAllocator( rtStackAllocator )
    , m_renderLogic( renderLogic )
    , m_renderQueueAllocator( rqStackAllocator )
    , m_audioRenderer( audioRenderer )
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
audio::AudioRenderer *          RenderLogicContext::GetAudioRenderer        () const
{
    return m_audioRenderer;
}

// ***************************
//
void                            RenderLogicContext::SetBoundRenderTarget    ( RenderTarget * target )
{
    m_boundRenderTarget = target;
}

} // bv
