#pragma once

#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {

class AlphaMaskRenderLogic : public NodeEffectRenderLogic
{
public:

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic ) override;

};

} // bv
