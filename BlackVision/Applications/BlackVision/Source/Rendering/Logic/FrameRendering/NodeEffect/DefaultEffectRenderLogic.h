#pragma once


#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"


namespace bv {

class DefaultEffectRenderLogic : public NodeEffectRenderLogic
{
public:

                    DefaultEffectRenderLogic    ( FrameRenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~DefaultEffectRenderLogic   ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

};

} //bv
