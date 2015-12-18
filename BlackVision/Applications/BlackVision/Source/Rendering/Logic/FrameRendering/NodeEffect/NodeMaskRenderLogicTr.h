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

    void                                RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, RenderTarget * maskRt, SceneNode * node );
    void                                RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

    BlitAlphaMaskFullscreenEffect *     AccessBlitAlphaMaskEffect   ( RenderTarget * rt, RenderTarget * alphaRt );

    void                                BlitWithMask                ( RenderLogicContext * ctx, RenderTarget * foregroundRt, RenderTarget * maskRt );
};

} // bv
