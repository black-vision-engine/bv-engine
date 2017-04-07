#include "stdafx.h"

#include "RenderLogicState.h"


namespace bv { namespace nrl {

// **************************
//
RenderLogicState::RenderLogicState   ( unsigned int width, unsigned int height )
    : m_initialized( false )
    , m_renderTargetAllocator( width, height )

{
    m_ctx.SetAllocator( &m_renderTargetAllocator );
    m_ctx.SetRenderQueueAllocator( &m_renderQueueAllocator );
}

// **************************
//
RenderContext *			    RenderLogicState::GetRenderContext		            ()
{
    return &m_ctx;
}

// **************************
//
RenderTargetStackAllocator *    RenderLogicState::GetRenderTargetStackAllocator    ()
{
    return &m_renderTargetAllocator;
}

// **************************
//
RenderQueueStackAllocator *     RenderLogicState::GetRenderQueueStackAllocator     ()
{
    return &m_renderQueueAllocator;
}

// **************************
//
bool                            RenderLogicState::IsInitialized                    () const
{
    return m_initialized;
}

// **************************
//
void                            RenderLogicState::Initialize                       ( Renderer * renderer, audio::AudioRenderer * audio )
{
    m_ctx.SetRenderer( renderer );
    m_ctx.SetAudio( audio );

    m_initialized = true;
}

} // nrl
} // bv
