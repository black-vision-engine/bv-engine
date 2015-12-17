#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"


namespace bv {

class BlitFullscreenEffect;
class RenderTarget;
class Renderer;


class AlphaMaskRenderLogicTr : public NodeEffectRenderLogicTr
{
private:

    BlitFullscreenEffect *          m_blitEffect;

public:

                    AlphaMaskRenderLogicTr                      ();
    virtual         ~AlphaMaskRenderLogicTr                     ();

    virtual void    RenderNode                                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    BlitFullscreenEffect *  AccessBlitAlphaEffect               ( RenderTarget * rt, float alpha );

    void                    BlitWithAlpha                       ( Renderer * renderer, RenderTarget * mainTarget, RenderTarget * alphaTarget, float alpha );

};

} // bv
