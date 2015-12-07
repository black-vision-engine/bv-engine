#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"


namespace bv {


class NodeMaskRenderLogicTr : public NodeEffectRenderLogicTr
{
public:

                    NodeMaskRenderLogicTr       ();
    virtual         ~NodeMaskRenderLogicTr      ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

};

} // bv
