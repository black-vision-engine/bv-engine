#include "stdafx.h"

#include "RenderLogicImpl.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { 

// **************************
//
RenderLogicImpl::RenderLogicImpl      ( unsigned int width, unsigned int height )
    : m_state( width, height )
    , m_renderedChannelsData( nullptr )
    , m_outputLogic( nullptr )
{
}

// **************************
//
RenderLogicImpl::~RenderLogicImpl     ()
{
    delete m_renderedChannelsData;
    delete m_outputLogic;
}

// **************************
//
void            RenderLogicImpl::HandleFrame       ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes )
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
OutputLogic *   RenderLogicImpl::GetOutputLogic    ()
{
    return m_outputLogic;
}

// **************************
//
RenderedChannelsData *  RenderLogicImpl::GetRenderedChannelsData   ()
{
    return m_renderedChannelsData;
}

// **************************
//
void            RenderLogicImpl::RenderQueued      ( const SceneVec & scenes )
{
    m_renderLogicCore.Render( scenes, m_renderedChannelsData, context( m_state ) );
}

// **************************
//
void            RenderLogicImpl::SetRenderedChannelsData ( RenderedChannelsData * rcd )
{
    assert( m_renderedChannelsData == nullptr );

    m_renderedChannelsData = rcd;
}

// **************************
//
void            RenderLogicImpl::SetOutputLogic          ( OutputLogic * outputLogic )
{
    assert( m_outputLogic == nullptr );

    m_outputLogic = outputLogic;
}

// **************************
//
RenderLogicState & RenderLogicImpl::AccessState           ()
{
    return m_state;
}

// **************************
//
RenderLogicImpl *  RenderLogicImpl::Create        ( RenderLogicDesc & desc )
{
    auto w = desc.GetMainWidth();
    auto h = desc.GetMainHeight();

    auto impl   = new RenderLogicImpl( w, h );
    
    auto alc    = impl->AccessState().GetRenderTargetStackAllocator();

    auto & aold = desc.AccessOutputLogicDesc();
    auto & rcdd = desc.AccessRenderedChannelsDataDesc();

    auto outLogic   = OutputLogic::Create( aold );
    auto renChan    = RenderedChannelsData::Create( rcdd, alc );
   
    impl->SetRenderedChannelsData( renChan );
    impl->SetOutputLogic( outLogic );

    return impl;
}


} //bv
