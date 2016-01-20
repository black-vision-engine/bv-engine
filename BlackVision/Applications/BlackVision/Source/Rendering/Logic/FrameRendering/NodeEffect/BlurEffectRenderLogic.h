#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

#include "Mathematics/glm_inc.h"

#include "CoreDEF.h"

namespace bv {

class BlurFullscreenEffect;
class RenderTarget;
class Renderer;


class BlurEffectRenderLogic : public NodeEffectRenderLogic
{
private:

    BlurFullscreenEffect *     m_blurEffect;

public:

                    BlurEffectRenderLogic       ();
    virtual         ~BlurEffectRenderLogic      ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    void                                RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, SceneNode * node );
    void                                RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

    BlurFullscreenEffect *              AccessBlurEffect            ( RenderTarget * rt, Float32 bs, bool vertical, bool normalize, Int32 blurKernelTypeVal );

    void                                ApplyBlurEffect             ( Renderer * renderer, RenderTarget * foregroundRt, Float32 bs, bool vertical, bool normalize, Int32 blurKernelTypeVal );
};

} // bv
