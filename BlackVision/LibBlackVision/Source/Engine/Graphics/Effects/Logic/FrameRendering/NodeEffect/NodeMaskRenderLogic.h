#pragma once

#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class BlitAlphaMaskFullscreenEffect;
class RenderTarget;
class Renderer;


class NodeMaskRenderLogic : public NodeEffectRenderLogic
{
private:

    BlitAlphaMaskFullscreenEffect *     m_blitAlphaMaskEffect;

public:

                    NodeMaskRenderLogic         ();
    virtual         ~NodeMaskRenderLogic        ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    void                                RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, RenderTarget * maskRt, SceneNode * node );
    void                                RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

    BlitAlphaMaskFullscreenEffect *     AccessBlitAlphaMaskEffect   ( RenderTarget * rt, RenderTarget * alphaRt, float alpha );

    void                                BlitWithMask                ( Renderer * renderer, RenderTarget * foregroundRt, RenderTarget * maskRt, float alpha );
};

} // bv