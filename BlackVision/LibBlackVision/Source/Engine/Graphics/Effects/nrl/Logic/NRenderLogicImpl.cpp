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
void            NRenderLogicImpl::RenderFrame       ( Renderer * renderer, audio::AudioRenderer * audio, SceneNode * sceneRoot )
{
    if( !m_state.IsInitialized() )
    {
        m_state.Initialize( renderer, audio );
    }

    renderer->PreDraw();

    auto renderResult = render_result( m_state );
    renderResult->UpdateOutputChannels();

    Render      ( sceneRoot );

    // FIXME: add an output renderTarget (possibly if necessary)
    // FIXME: nrl - add audio somewhere in this class
    output_logic( m_state )->ProcessFrameData( ctx( m_state ), renderResult );

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
void             NRenderLogicImpl::Render           ( SceneNode * sceneRoot )
{
	m_renderLogicCore.Render( m_state.GetRenderResult(), sceneRoot, m_state.GetRenderContext() );
    m_audioLogicCore.RenderAudio( sceneRoot, m_state.GetRenderContext() );
}

} //nrl
} //bv
