#include "stdafx.h"

#include "NRenderLogicImpl.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicImpl::NRenderLogicImpl  ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_state( width, height, numTrackedRenderTargetsPerOutputType ) 
{
}

// **************************
//
void            NRenderLogicImpl::HandleFrame       ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes )
{
    // 0. Make sure that the state is properly initialized
    if( !m_state.IsInitialized() )
    {
        m_state.Initialize( renderer, audio );
    }

    // 1. Access RenderedChannelsData associated with this RenderLogic instance and update (per frame) output buffers
    auto renderdata     = rendered_channels_data( m_state );     
    renderdata->UpdateRenderChannels();

    // 2. Low level renderer per frame initialization
    renderer->PreDraw();

    // 3. FIXME: nrl - RenderQueued is only one possible way of rendering - this one needs additional inspection
    RenderQueued( scenes, renderdata );

    // 4. Low lecel rendere per frame cleanup
    renderer->PostDraw();

    // 5. Handle frame data rendered during this call and all logic associated with custom outputs
    output_logic( m_state )->ProcessFrameData( context( m_state ) );
}

// **************************
//
OutputLogic *   NRenderLogicImpl::GetOutputLogic    ()
{
    return output_logic( m_state );
}

// **************************
//
RenderedChannelsData *  NRenderLogicImpl::GetRenderedChannelsData   ()
{
    return rendered_channels_data( m_state );
}

// **************************
//
void            NRenderLogicImpl::RenderQueued      ( const SceneVec & scenes, RenderedChannelsData * result )
{
    auto ctx = context( m_state );
    
    m_renderLogicCore.Render( scenes, result, ctx );
}

} //nrl
} //bv
