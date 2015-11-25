#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class AlphaMaskRenderLogic : public NodeEffectRenderLogic
{
public:

                    AlphaMaskRenderLogic        ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~AlphaMaskRenderLogic       ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

};

} // bv
