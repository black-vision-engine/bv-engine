#pragma once


#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {

class WireframeRenderLogic : public NodeEffectRenderLogic
{
public:

                    WireframeRenderLogic        ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~WireframeRenderLogic       ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

};

} //bv
