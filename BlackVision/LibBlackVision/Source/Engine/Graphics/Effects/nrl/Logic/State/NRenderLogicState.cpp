#include "stdafx.h"

#include "NRenderLogicState.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicState::NRenderLogicState   ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType, unsigned int sharedMemScaleFactor )
    : m_initialized( false )
    , m_renderTargetAllocator( width, height )
    , m_outputLogic( width, height, sharedMemScaleFactor, &m_renderTargetAllocator, numTrackedRenderTargetsPerOutputType /* pass appropriate constructor arguments here */ )

{
    // FIXME: nrl - THIS IS ONE HUGE HACK - IMPLEMENT VALID STATIC CONFIGURATION INIT MECHANISM AND USE IT - BASED ON PAWELEK'S REQUIREMENTS
    {
        // Enable single render channel
        auto res = m_outputLogic.AccessRenderResult();
        res->SetIsActive( RenderChannelType::RCT_OUTPUT_1, true );

        // Enable required outputs
        m_outputLogic.EnableOutput( CustomOutputType::COT_PREVIEW );    // FIXME: nrl - make sure that when this output is disabled, preview is not rendered
        //m_outputLogic.EnableOutput( CustomOutputType::COT_STREAM_SHM ); // FIXME: nrl - make sure that when this output is disabled, shared memory oputput is not used
        m_outputLogic.EnableOutput( CustomOutputType::COT_VIDEO_HD );      // FIXME: nrl - make sure that when this output is disabled, video oputput is not used
        //m_outputLogic.EnableOutput( CustomOutputType::COT_VIDEO_SD );      // FIXME: nrl - make sure that when this output is disabled, video oputput is not used
    }

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
