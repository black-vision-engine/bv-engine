#include "stdafx.h"

#include "Preview.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
Preview::Preview     ()
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
Preview::~Preview    ()
{
    delete m_defaultBlitEffect;
    delete m_blitEffectShowAplhaRGB;
}

// *********************************
//
void    Preview::ShowFrame( NRenderContext * ctx, const RenderResult * result )
{
    // FIXME: prepare one texture combining all required outputs and blit it to the output render target
    // FIXME: call BlitToPreview here
    //auto rt = result.GetRenderOutputChannel( RenderOutputChannelType::ROCT_FIRST_OUTPUT );
    // FIXME: all outputs rendered next to each other (scaled down)
    // FIXME: shader memory gets all outputs rendered to consecutive buffers in the memory
    // m_blitEffect->Render( rt, nullptr );

    DefaultShow( ctx, result->GetActiveRenderTarget( RenderOutputChannelType::ROCT_OUTPUT_1 ) );

    // Make sure that local preview is displayed properly
    renderer( ctx )->DisplayColorBuffer();

}

// *********************************
//
void    Preview::SetShowDefault  ()
{
    m_activeBlit = m_defaultBlitEffect;
}

// *********************************
//
void    Preview::SetShowAlpha    ()
{
    m_activeBlit = m_blitEffectShowAplhaRGB;
}

// *********************************
//
void    Preview::DefaultShow ( NRenderContext * ctx, const RenderTarget * rt )
{
    m_activeRenderOutput.SetEntry( 0, rt );

    m_activeBlit->Render( ctx, m_activeRenderOutput );
}

} //nrl
} //bv
