#pragma once

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"


namespace bv {

class AlphaMaskRenderLogicTr : public NodeEffectRenderLogicTr
{
public:

                    AlphaMaskRenderLogicTr      ();
    virtual         ~AlphaMaskRenderLogicTr     ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

};

} // bv
