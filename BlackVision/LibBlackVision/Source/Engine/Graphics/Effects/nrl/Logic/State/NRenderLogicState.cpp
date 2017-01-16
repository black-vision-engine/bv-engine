#include "stdafx.h"

#include "NRenderLogicState.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicState::NRenderLogicState   ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType, unsigned int sharedMemScaleFactor )
    : m_initialized( false )
    , m_renderTargetAllocator( width, height )
    , m_outputLogic( &m_renderTargetAllocator, numTrackedRenderTargetsPerOutputType /* pass appropriate constructor arguments here */ )
{
    // FIXME: nrl - THIS IS ONE HUGE HACK - IMPLEMENT VALID STATIC CONFIGURATION INIT MECHANISM AND USE IT
    {
        auto res = m_outputLogic.AccessRenderResult();

        m_outputLogic.EnableOutput( CustomOutputType::COT_PREVIEW ); // FIXME: nrl - make sure that when this output is disabled, preview is not rendered
        res->SetIsActive( RenderChannelType::RCT_OUTPUT_1, true );
    }

    { sharedMemScaleFactor; } // FIXME: pass it to the output logic

    m_ctx.SetAllocator( &m_renderTargetAllocator );
    m_ctx.SetRenderQueueAllocator( &m_renderQueueAllocator );
}

// **************************
//
OutputLogic *               NRenderLogicState::GetOutputLogic      ()
{
    return &m_outputLogic;
}

// **************************
//
NRenderContext *			NRenderLogicState::GetRenderContext		()
{
    return &m_ctx;
}

// **************************
//
bool                        NRenderLogicState::IsInitialized       () const
{
    return m_initialized;
}

// **************************
//
void                        NRenderLogicState::Initialize          ( Renderer * renderer, audio::AudioRenderer * audio )
{
    m_ctx.SetRenderer( renderer );
    m_ctx.SetAudio( audio );

    m_initialized = true;
}

} // nrl
} // bv
