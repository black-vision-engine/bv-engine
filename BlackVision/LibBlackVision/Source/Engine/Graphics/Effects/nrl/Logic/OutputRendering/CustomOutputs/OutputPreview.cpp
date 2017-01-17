#include "stdafx.h"

#include "OutputPreview.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputPreview::OutputPreview            ()
    : m_defaultBlitEffect( nullptr )
    , m_blitEffectShowAplhaRGB( nullptr )
    , m_activeRenderOutput( 1 )
{
    m_defaultBlitEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_SIMPLE_BLIT );
    m_blitEffectShowAplhaRGB = CreateFullscreenEffect( NFullscreenEffectType::NFET_PREVIEW_ALPHA_AND_RGB ); 

    m_activeBlit = m_defaultBlitEffect;
}

// *********************************
//
OutputPreview::~OutputPreview           ()
{
    delete m_defaultBlitEffect;
    delete m_blitEffectShowAplhaRGB;
}

// *********************************
//
void    OutputPreview::ProcessFrameData ( NRenderContext * ctx, RenderResult * result )
{
    // FIXME: nrl - default logic uses only RenderChannelType::RCT_OUTPUT_1 result channel to show the results
    // FIXME: ntl - implement more logic here
    auto rct = RenderChannelType::RCT_OUTPUT_1;
	rct;
    assert( result->IsActive( rct ) && result->ContainsValidData( rct ) );

    // FIXME: nrl - DefaultShow is only a very siple way of showing rendered result on preview - ask Pawelek about other possibilities
    DefaultShow( ctx, result->GetActiveRenderTarget( RenderChannelType::RCT_OUTPUT_1 ) );

    // Make sure that local preview is displayed properly
    renderer( ctx )->DisplayColorBuffer();
}

// *********************************
//
void    OutputPreview::SetShowDefault  ()
{
    m_activeBlit = m_defaultBlitEffect;
}

// *********************************
//
void    OutputPreview::SetShowAlpha    ()
{
    m_activeBlit = m_blitEffectShowAplhaRGB;
}

// *********************************
//
void    OutputPreview::DefaultShow ( NRenderContext * ctx, const RenderTarget * rt )
{
    m_activeRenderOutput.SetEntry( 0, rt );

    m_activeBlit->Render( ctx, m_activeRenderOutput );
}

} //nrl
} //bv
