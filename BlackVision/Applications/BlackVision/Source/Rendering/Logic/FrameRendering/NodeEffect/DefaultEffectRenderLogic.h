#pragma once


#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class DefaultEffectRenderLogic : public NodeEffectRenderLogic
{
public:

                    DefaultEffectRenderLogic    ();
    virtual         ~DefaultEffectRenderLogic   ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

};

} //bv
