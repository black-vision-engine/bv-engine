#include "stdafx.h"

#include "NRenderLogicImpl.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicImpl::NRenderLogicImpl      ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_state( width, height, numTrackedRenderTargetsPerOutputType )
    , m_renderedChannelsData( nullptr )
    , m_outputLogic( nullptr )
{
}

// **************************
//
NRenderLogicImpl::~NRenderLogicImpl     ()
{
    delete m_renderedChannelsData;
    delete m_outputLogic;
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

    // 4. Low level rendere per frame cleanup
    renderer->PostDraw();

    // 5. Handle frame data rendered during this call and all logic associated with custom outputs
    output_logic( m_state )->ProcessFrameData( context( m_state ) );
}

// **************************
//
OutputLogic *   NRenderLogicImpl::GetOutputLogic    ()
{
    return &m_outputLogic;
}

// **************************
//
RenderedChannelsData *  NRenderLogicImpl::GetRenderedChannelsData   ()
{
    return &m_renderedChannelsData;
}

// **************************
//
void            NRenderLogicImpl::RenderQueued      ( const SceneVec & scenes, RenderedChannelsData * result )
{
    auto ctx = context( m_state );
    
    m_renderLogicCore.Render( scenes, result, ctx );
}

// **************************
//
void            NRenderLogicImpl::SetRenderedChannelsData ( RenderedChannelsData * rcd )
{
    assert( m_renderedChannelsData == nullptr );

    m_renderedChannelsData = rcd;
}

// **************************
//
void            NRenderLogicImpl::SetOutputLogic          ( OutputLogic * outputLogic )
{
    assert( m_outputLogic == nullptr );

    m_outputLogic = outputLogic;
}

// **************************
//
NRenderLogicState & NRenderLogicImpl::AccessState           ()
{
    reutrn m_state;
}

// **************************
//
NRenderLogicImpl *  NRenderLogicImpl::Create        ( const NRenderLogicDesc & desc )
{
    auto w = desc.GetMainWidth();
    auto h = desc.GetMainHeight();
    auto n = desc.AccessRenderedChannelsDataDesc().GetNumTrackedRenderTargets();
    
    auto impl = new NRenderLogicImpl( w, h, n );


    impl->SetRenderedChannelsData( nullptr );
    impl->SetOutputLogic( nullptr );

    return nullptr;
}

} //nrl
} //bv
