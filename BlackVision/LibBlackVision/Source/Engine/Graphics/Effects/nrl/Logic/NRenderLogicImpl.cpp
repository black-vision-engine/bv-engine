#include "stdafx.h"

#include "NRenderLogicImpl.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicImpl::NRenderLogicImpl  ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType, unsigned int sharedMemScaleFactor )
    : m_state( width, height, numTrackedRenderTargetsPerOutputType, sharedMemScaleFactor ) 
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

    // 1. Access output logic associated with this RenderLogic instance and update (per frame) output buffers
    auto outputLogic = output_logic( m_state );
    outputLogic->UpdateRenderChannels();

    // 2. Low level renderer per frame initialization
    renderer->PreDraw();

    // 3. FIXME: nrl - RenderQueued is only one possible way of rendering - this one needs additional inspection
    RenderQueued( scenes, outputLogic->AccessRenderResult() );

    // 4. Low lecel rendere per frame cleanup
    renderer->PostDraw();

    // 5. Handle frame data rendered during this call and all logic associated with custom outputs
    outputLogic->ProcessFrameData( context( m_state ) );
}

// **************************
//
OutputLogic *   NRenderLogicImpl::GetOutputLogic    ()
{
    return output_logic( m_state );
}

// **************************
//
void            NRenderLogicImpl::RenderQueued      ( const SceneVec & scenes, RenderResult * result )
{
    auto ctx = context( m_state );
    
    m_renderLogicCore.Render( scenes, result, ctx );
}

} //nrl
} //bv
