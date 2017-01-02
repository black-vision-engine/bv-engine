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
void            NRenderLogicImpl::RenderFrame       ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes )
{
    if( !m_state.IsInitialized() )
    {
        m_state.Initialize( renderer, audio );
    }

    renderer->PreDraw();

    auto renderResult = render_result( m_state );
    renderResult->UpdateOutputChannels();

    RenderQueued( scenes, renderResult );
    // FIXME: nrl - add audio somewhere in this class
    output_logic( m_state )->ProcessFrameData( context( m_state ), renderResult );

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
    auto r = renderer( ctx );

    auto outputRenderTarget = result->GetActiveRenderTarget( RenderOutputChannelType::ROCT_FIRST_OUTPUT );

    enable( ctx, outputRenderTarget );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit;

    for( auto & scene : scenes )
    {
        r->Performance().AverageScenePerformanceData( scene );
        r->SetCamera( scene->GetCamera() );
        r->EnableScene( scene );
 
        auto root = scene->GetRoot();

        auto rq = queue_allocator( ctx )->Allocate();
        rq->QueueNodeSubtree( root, ctx );
        rq->Render( ctx );

        queue_allocator( ctx )->Free();

    //    RenderGridLines( scene, ctx );          // FIXME: Use some generic solution when other editor helper object apear in engine.
        m_audioLogicCore.RenderAudio( root, ctx );

    }

    disableBoundRT( ctx );
}

// **************************
//
void            NRenderLogicImpl::Render            ( SceneNode * sceneRoot )
{
	m_renderLogicCore.Render( m_state.GetRenderResult(), sceneRoot, m_state.GetRenderContext() );
    m_audioLogicCore.RenderAudio( sceneRoot, m_state.GetRenderContext() );
}

} //nrl
} //bv
