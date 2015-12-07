#pragma once


#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"


namespace bv {

class WireframeEffect;
class Renderer;


class WireframeRenderLogicTr : public NodeEffectRenderLogicTr
{
private:

    WireframeEffect     *   m_effect;

public:

                    WireframeRenderLogicTr      ();
    virtual         ~WireframeRenderLogicTr     ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) override;

private:

    void            RenderWireframeOverlay      ( Renderer * renderer, SceneNode * node );

    void            EnableWireframeEffect       ( Renderer * renderer, SceneNode * node );

    void            DrawWirefreameNodeOnly      ( Renderer * renderer, SceneNode * node );
    void            DrawWireframeChildren       ( Renderer * renderer, SceneNode * node );

};


} //bv
