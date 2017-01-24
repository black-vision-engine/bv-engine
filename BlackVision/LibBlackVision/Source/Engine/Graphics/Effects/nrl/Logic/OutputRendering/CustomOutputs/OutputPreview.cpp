#include "stdafx.h"

#include "OutputPreview.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputPreview::OutputPreview            ( unsigned int width, unsigned int height )
    : OutputInstance( width, height )
    , m_mixChannelsEffect( nullptr )
    , m_activeRenderOutput( 1 )
{
    m_mixChannelsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_MIX_CHANNELS ); 
}

// *********************************
//
OutputPreview::~OutputPreview           ()
{
    delete m_mixChannelsEffect;
}

// *********************************
//
void    OutputPreview::ProcessFrameData ( NRenderContext * ctx, RenderResult * input )
{
    // FIXME: nrl - this is a bit of an overkill, but let's update it every frame here
    UpdateEffectValues();

    auto rct = GetActiveRenderChannel();
    assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );

    // FIXME: nrl - DefaultShow is only a very siple way of showing rendered result on preview - ask Pawelek about other possibilities
    DefaultShow( ctx, input->GetActiveRenderTarget( rct ) );

    // Make sure that local preview is displayed properly
    renderer( ctx )->DisplayColorBuffer();
}

// *********************************
//
void    OutputPreview::DefaultShow ( NRenderContext * ctx, const RenderTarget * rt )
{
    m_activeRenderOutput.SetEntry( 0, rt );

    m_mixChannelsEffect->Render( ctx, m_activeRenderOutput );
}

// *********************************
//
void    OutputPreview::UpdateEffectValues      ()
{
    auto state = m_mixChannelsEffect->GetState();
    
    auto mappingVal = state->GetValueAt( 0 ); assert( mappingVal->GetName() == "channelMapping" );
    auto maskVal    = state->GetValueAt( 1 ); assert( maskVal->GetName() == "channelMask" );

    auto mapping    = GetChannelMapping();
    auto mask       = GetChannelMask();

    QueryTypedValue< ValueIntPtr >( mappingVal )->SetValue( mapping );
    QueryTypedValue< ValueVec4Ptr >( maskVal )->SetValue( mask );
}

} //nrl
} //bv
