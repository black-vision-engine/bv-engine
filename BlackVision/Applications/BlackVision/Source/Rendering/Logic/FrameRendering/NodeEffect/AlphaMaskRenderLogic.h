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

    void                    Start                               ( RenderLogicContext * ctx );
    void                    Finalize                            ( RenderLogicContext * ctx );

    BlitFullscreenEffect *  AccessBlitAlphaEffect               ( RenderTarget * rt, float alpha );

    void                    BlitWithAlpha                       ( RenderLogicContext * ctx, RenderTarget * alphaTarget, float alpha );
};

} // bv
