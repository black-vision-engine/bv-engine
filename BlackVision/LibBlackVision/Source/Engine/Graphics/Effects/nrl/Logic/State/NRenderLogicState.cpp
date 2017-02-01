#include "stdafx.h"

#include "NRenderLogicState.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicState::NRenderLogicState   ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_initialized( false )
    , m_renderTargetAllocator( width, height )
    , m_renderedChannelsData( &m_renderTargetAllocator, numTrackedRenderTargetsPerOutputType )
    , m_outputLogic( &m_renderTargetAllocator, numTrackedRenderTargetsPerOutputType )

{
    m_ctx.SetAllocator( &m_renderTargetAllocator );
    m_ctx.SetRenderQueueAllocator( &m_renderQueueAllocator );
}

// **************************
//
OutputLogic *               NRenderLogicState::GetOutputLogic           ()
{
    return &m_outputLogic;
}

// **************************
//
NRenderContext *			NRenderLogicState::GetRenderContext		    ()
{
    return &m_ctx;
}

// **************************
//
RenderedChannelsData *      NRenderLogicState::GetRenderedChannelsData  ()
{
    return &m_renderedChannelsData;
}

// **************************
//
bool                        NRenderLogicState::IsInitialized            () const
{
    return m_initialized;
}

// **************************
//
void                        NRenderLogicState::Initialize               ( Renderer * renderer, audio::AudioRenderer * audio )
{
    m_ctx.SetRenderer( renderer );
    m_ctx.SetAudio( audio );

    m_initialized = true;
}

} // nrl
} // bv
