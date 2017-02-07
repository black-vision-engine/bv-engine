#include "stdafx.h"

#include "NRenderLogicImpl.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicImpl::NRenderLogicImpl      ( unsigned int width, unsigned int height )
    : m_state( width, height )
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
    m_renderedChannelsData->UpdateRenderChannels();

    // 2. Low level renderer per frame initialization
    renderer->PreDraw();

    // 3. FIXME: nrl - RenderQueued is only one possible way of rendering - this one needs additional inspection
    RenderQueued( scenes );

    // 4. Low level rendere per frame cleanup
    renderer->PostDraw();

    // 5. Handle frame data rendered during this call and all logic associated with custom outputs
    m_outputLogic->ProcessFrameData( context( m_state ), m_renderedChannelsData );
}

// **************************
//
OutputLogic *   NRenderLogicImpl::GetOutputLogic    ()
{
    return m_outputLogic;
}

// **************************
//
RenderedChannelsData *  NRenderLogicImpl::GetRenderedChannelsData   ()
{
    return m_renderedChannelsData;
}

// **************************
//
void            NRenderLogicImpl::RenderQueued      ( const SceneVec & scenes )
{
    m_renderLogicCore.Render( scenes, m_renderedChannelsData, context( m_state ) );
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
    return m_state;
}

// **************************
//
NRenderLogicImpl *  NRenderLogicImpl::Create        ( NRenderLogicDesc & desc )
{
    auto w = desc.GetMainWidth();
    auto h = desc.GetMainHeight();

    auto impl   = new NRenderLogicImpl( w, h );
    
    auto alc    = impl->AccessState().GetRenderTargetStackAllocator();

    auto & aold = desc.AccessOutputLogicDesc();
    auto & rcdd = desc.AccessRenderedChannelsDataDesc();

    auto outLogic   = OutputLogic::Create( aold );
    auto renChan    = RenderedChannelsData::Create( rcdd, alc );
   
    impl->SetRenderedChannelsData( renChan );
    impl->SetOutputLogic( outLogic );

    return impl;
}

} //nrl
} //bv
