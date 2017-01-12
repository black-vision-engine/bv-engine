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
void            NRenderLogicImpl::RenderFrame       ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes )
{
    if( !m_state.IsInitialized() )
    {
        m_state.Initialize( renderer, audio );
    }

    renderer->PreDraw();

    auto renderResult = render_result( m_state );
    renderResult->UpdateOutputChannels( scenes );

    RenderQueued( scenes, renderResult );

    // FIXME: nrl - add audio somewhere in this class
    output_logic( m_state )->ProcessFrameData( context( m_state ), renderResult, (unsigned int)scenes.size() );
    

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// **************************
//
Preview *       NRenderLogicImpl::GetPreview        ()
{
    return output_logic( m_state )->GetPreview();
}

// **************************
//
VideoOutput *   NRenderLogicImpl::GetVideoOutput    ()
{
    return output_logic( m_state )->GetVideoOutput();
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
