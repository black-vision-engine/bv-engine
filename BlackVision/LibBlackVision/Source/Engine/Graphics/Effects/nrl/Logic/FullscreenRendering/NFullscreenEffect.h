#pragma once

#include <string>

#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectComponentState.h"


namespace bv { 

class Camera;
class Renderer;
class RenderTarget;

namespace nrl {

class NRenderContext;

enum class NFullscreenEffectType : int
{
    NFET_SIMPLE_BLIT = 0,
    NFET_PREVIEW_ALPHA_AND_RGB,
    NFET_BLIT_WITH_ALPHA,
    NFET_BLIT_WITH_ALPHA_MASK,
    NFET_INTERLACE,
    NFET_MIX_CHANNELS,
    NFET_VIDEO_OUTPUT,
    NFET_BLUR,
	NFET_LIGHT_SCATTERING,
    NFET_SHADOW,
	NFET_GLOW,
	NFET_SOFT_MASK,
    //NFET_OUTLINE
    //TODO: and so on

    NFET_TOTAL
};

const std::string GFetFile[] = {
								"blit_no_alpha.frag",
                                "blit_preview_alpha_and_rgb.frag",
								"blit_alpha.frag",
								"blit_mask_alpha.frag",
								"interlace.frag",
								"mixchannels.frag",
								"videooutput.frag",
								"blur.frag",
								"light_scattering.frag",
								"shadow.frag",
								"glow.frag",
								"soft_mask.frag"
							   };

class NFullscreenEffectInstance;
class NRenderedData;

class NFullscreenEffect
{
private:
    
    NFullscreenEffectInstance *         m_fseInst;

    Camera *                            m_rendererCamera;

    NFullscreenEffectComponentStatePtr  m_state;

public:

            NFullscreenEffect   ( NFullscreenEffectInstance * fseInst, NFullscreenEffectComponentStatePtr state );
            ~NFullscreenEffect  ();

    // Render to currently bound render target
    void    Render              ( NRenderContext * ctx );
    void    Render              ( NRenderContext * ctx, const NRenderedData & dynamicInput );

    // Render to a specified bound render target
    void    Render              ( NRenderContext * ctx, const RenderTarget * output );
    void    Render              ( NRenderContext * ctx, const RenderTarget * output, const NRenderedData & dynamicInput );

    NFullscreenEffectComponentStatePtr GetState     ();

    void                AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const;

private:

    void    SetFullscreenCamera ( Renderer * renderer );
    void    SetRendererCamera   ( Renderer * renderer );

};

} //nrl
} //bv
