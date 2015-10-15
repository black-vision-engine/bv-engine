#pragma once


#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {

class DefaultEffectRenderLogic : public NodeEffectRenderLogic
{
public:

                    DefaultEffectRenderLogic    ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~DefaultEffectRenderLogic   ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

};

} //bv
