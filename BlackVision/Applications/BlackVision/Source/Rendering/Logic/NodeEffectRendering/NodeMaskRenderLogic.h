#pragma once

#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {


class NodeMaskRenderLogic : public NodeEffectRenderLogic
{
public:

                    NodeMaskRenderLogic         ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~NodeMaskRenderLogic        ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

};

} // bv