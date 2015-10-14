#pragma once


#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {

class DefaultEffectRenderLogic : public NodeEffectRenderLogic
{
public:

                    DefaultEffectRenderLogic    ();
    virtual         ~DefaultEffectRenderLogic   ();

    virtual void    Render                      ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic ) override;

};

} //bv
