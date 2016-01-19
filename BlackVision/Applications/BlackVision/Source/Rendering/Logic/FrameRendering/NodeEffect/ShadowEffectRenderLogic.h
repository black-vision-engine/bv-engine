#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Mathematics/glm_inc.h"

namespace bv {

class ShadowFullscreenEffect;
class BlurFullscreenEffect;
class RenderTarget;
class Renderer;


class ShadowEffectRenderLogic : public NodeEffectRenderLogic
{
private:

    ShadowFullscreenEffect *    m_shadowEffect;
    BlurFullscreenEffect *      m_blurEffect;

public:

                    ShadowEffectRenderLogic     ();
    virtual         ~ShadowEffectRenderLogic    ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    void                                RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, SceneNode * node );
    void                                RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

    ShadowFullscreenEffect *            AccessShadowEffect          ( Texture2DPtr tex, Texture2DPtr bluredTex, const glm::vec4 & color, const glm::vec2 & shift );

    void                                AddShadowEffect             ( Renderer * renderer, Texture2DPtr tex, Texture2DPtr bluredTex, const glm::vec4 & color, const glm::vec2 & shift );

    BlurFullscreenEffect *              AccessBlurEffect            ( RenderTarget * rt, float bs, bool vertical, bool normalize );

    void                                ApplyBlurEffect             ( Renderer * renderer, RenderTarget * foregroundRt, float bs, bool vertical, bool normalize );
};

} // bv
