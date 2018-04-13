#pragma once

#include <string>

#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectComponentState.h"


namespace bv { 

class Camera;
class Renderer;
class RenderTarget;



class RenderContext;

enum class FullscreenEffectType : int
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
    NFET_COLOR_CORRECTION,
    NFET_COLOR_BALANCE,
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
                                "soft_mask.frag",
                                "color_correction.frag",
                                "color_balance.frag"
                               };

class FullscreenEffectInstance;
class RenderedData;

class FullscreenEffect
{
private:
    
    FullscreenEffectInstance *         m_fseInst;

    Camera *                            m_rendererCamera;

    FullscreenEffectComponentStatePtr  m_state;

public:

            FullscreenEffect   ( FullscreenEffectInstance * fseInst, FullscreenEffectComponentStatePtr state );
            ~FullscreenEffect  ();

    // Render to currently bound render target
    void    Render              ( RenderContext * ctx );
    void    Render              ( RenderContext * ctx, const RenderedData & dynamicInput );

    // Render to a specified bound render target
    void    Render              ( RenderContext * ctx, const RenderTarget * output );
    void    Render              ( RenderContext * ctx, const RenderTarget * output, const RenderedData & dynamicInput );

    FullscreenEffectComponentStatePtr GetState     ();

    void                AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const;

private:

    void    SetFullscreenCamera ( Renderer * renderer );
    void    SetRendererCamera   ( Renderer * renderer );

};


} //bv
