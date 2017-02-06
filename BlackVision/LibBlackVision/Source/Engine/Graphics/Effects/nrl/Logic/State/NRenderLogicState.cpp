#include "stdafx.h"

#include "NRenderLogicState.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicState::NRenderLogicState   ( unsigned int width, unsigned int height )
    : m_initialized( false )
    , m_renderTargetAllocator( width, height )

{
    m_ctx.SetAllocator( &m_renderTargetAllocator );
    m_ctx.SetRenderQueueAllocator( &m_renderQueueAllocator );
}

// **************************
//
NRenderContext *			    NRenderLogicState::GetRenderContext		            ()
{
    return &m_ctx;
}

// **************************
//
RenderTargetStackAllocator *    NRenderLogicState::GetRenderTargetStackAllocator    ()
{
    return &m_renderTargetAllocator;
}

// **************************
//
RenderQueueStackAllocator *     NRenderLogicState::GetRenderQueueStackAllocator     ()
{
    return &m_renderQueueAllocator;
}

// **************************
//
bool                            NRenderLogicState::IsInitialized                    () const
{
    return m_initialized;
}

// **************************
//
void                            NRenderLogicState::Initialize                       ( Renderer * renderer, audio::AudioRenderer * audio )
{
    m_ctx.SetRenderer( renderer );
    m_ctx.SetAudio( audio );

    m_initialized = true;
}

} // nrl
} // bv
