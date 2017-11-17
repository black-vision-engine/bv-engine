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
    , m_inputLogic( new InputLogic )
{}

// **************************
//
RenderLogicImpl::~RenderLogicImpl     ()
{
    delete m_renderedChannelsData;
    delete m_outputLogic;
    delete m_inputLogic;
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

    // 1. Process all input sources
    m_inputLogic->ProcessInputs( context( m_state ) );

    // 2. Access RenderedChannelsData associated with this RenderLogic instance and update (per frame) output buffers
    m_renderedChannelsData->UpdateRenderChannels();

    // 3. Low level renderer per frame initialization
    renderer->PreDraw();

    // 4. FIXME: nrl - RenderQueued is only one possible way of rendering - this one needs additional inspection
    RenderQueued( scenes );

    // 5. In edit mode render depth buffer from current scene and gizmos after it.
    if( m_state.IsEditMode() )
    {
        RenderDepth( scenes );
        RenderGizmos( scenes );
        BlitGizmoTargets();
    }

    // 6. Low level rendere per frame cleanup
    renderer->PostDraw();

    // 7. Handle frame data rendered during this call and all logic associated with custom outputs
    m_outputLogic->ProcessFrameData( context( m_state ), m_renderedChannelsData );
}

// **************************
//
OutputLogic *           RenderLogicImpl::GetOutputLogic             ()
{
    return m_outputLogic;
}

// ***********************
//
InputLogic *            RenderLogicImpl::GetInputLogic              ()
{
    return m_inputLogic;
}

// **************************
//
RenderedChannelsData *  RenderLogicImpl::GetRenderedChannelsData    ()
{
    return m_renderedChannelsData;
}

// **************************
//
void            RenderLogicImpl::RenderQueued           ( const SceneVec & scenes )
{
    m_renderLogicCore.Render( scenes, m_renderedChannelsData, context( m_state ) );
}

// ***********************
//
void            RenderLogicImpl::RenderDepth            ( const SceneVec & scenes )
{
    m_renderLogicCore.RenderDepth( scenes, m_renderedChannelsData, context( m_state ) );
}

// ***********************
//
void            RenderLogicImpl::RenderGizmos           ( const SceneVec & scenes )
{
    m_renderLogicCore.RenderGizmos( scenes, m_renderedChannelsData, context( m_state ) );
}

// ***********************
//
void            RenderLogicImpl::BlitGizmoTargets       ()
{
    m_renderLogicCore.BlitGizmoTargets( m_renderedChannelsData, context( m_state ) );
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

// ***********************
//
void                RenderLogicImpl::SwitchEditMode     ( bool editMode )
{
    m_state.SwitchEditMode( editMode );


    // FIXME: Deallocate gizmo render target when switching from edit mode to production mode.
    // FIZME: The problem is const renderChannel :(
    //if( !editMode )
    //{
    //    for( int i = 0; i < (int)RenderChannelType::RCT_TOTAL; ++i )
    //    {
    //        auto renderChannel = m_renderedChannelsData->GetRenderChannel( i );
    //        if( renderChannel )
    //            renderChannel->DeallocGizmoRT();
    //    }
    //}
}


} //bv
