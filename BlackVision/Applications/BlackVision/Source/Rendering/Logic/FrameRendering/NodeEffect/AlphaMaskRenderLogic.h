#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class BlitFullscreenEffect;
class RenderTarget;
class Renderer;


class AlphaMaskRenderLogic : public NodeEffectRenderLogic
{
private:

    BlitFullscreenEffect *          m_blitEffect;

public:

                    AlphaMaskRenderLogic                        ();
    virtual         ~AlphaMaskRenderLogic                       ();

    virtual void    RenderNode                                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    RenderTarget *          RenderToRenderTarget                ( RenderLogicContext * ctx, SceneNode * node );

    BlitFullscreenEffect *  AccessBlitAlphaEffect               ( RenderTarget * rt, float alpha );

    void                    BlitWithAlpha                       ( Renderer * renderer, RenderTarget * alphaTarget, float alpha );
};

} // bv
