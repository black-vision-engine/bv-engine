#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

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

    ShadowFullscreenEffect *            AccessShadowEffect          ( RenderTarget * rt, const glm::vec4 & color, const glm::vec2 & shift, float bs );

    void                                AddShadowEffect             ( Renderer * renderer, RenderTarget * foregroundRt, const glm::vec4 & color, const glm::vec2 & shift, float bs );

    BlurFullscreenEffect *              AccessBlurEffect            ( RenderTarget * rt, float bs, bool vertical );

    void                                ApplyBlurEffect             ( Renderer * renderer, RenderTarget * foregroundRt, float bs, bool vertical );
};

} // bv
