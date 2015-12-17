#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"


namespace bv {

class BlitAlphaMaskFullscreenEffect;
class RenderTarget;
class Renderer;


class NodeMaskRenderLogicTr : public NodeEffectRenderLogicTr
{
private:

    BlitAlphaMaskFullscreenEffect *     m_blitAlphaMaskEffect;

public:

                    NodeMaskRenderLogicTr       ();
    virtual         ~NodeMaskRenderLogicTr      ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    BlitAlphaMaskFullscreenEffect *     AccessBlitAlphaMaskEffect   ( RenderTarget * rt, RenderTarget * alphaRt, float alpha );

    void                                BlitWithAlphaMask           ( Renderer * renderer, RenderTarget * mainTarget, RenderTarget * foregroundTarget, RenderTarget * alphaTarget, float alpha );

};

} // bv
