#pragma once


#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"


namespace bv {

class WireframeRenderLogic : public NodeEffectRenderLogic
{
public:

                    WireframeRenderLogic        ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic );
    virtual         ~WireframeRenderLogic       ();

    virtual void    RenderNode                  ( Renderer * renderer, SceneNode * node ) override;

private:

    void            RenderWireframeOverlay      ( Renderer * renderer, SceneNode * node );

    void            EnableWireframeEffect       ( Renderer * renderer );

    void            DrawWirefreameNodeOnly      ( Renderer * renderer, SceneNode * node );
    void            DrawWireframeChildren       ( Renderer * renderer, SceneNode * node );

    void            DisableWireframeEffect      ( Renderer * renderer );

};


} //bv
