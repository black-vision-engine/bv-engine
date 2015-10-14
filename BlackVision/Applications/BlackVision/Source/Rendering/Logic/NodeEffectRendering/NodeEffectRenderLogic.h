#pragma once

namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;

class NodeEffectRenderLogic
{
public:

                    NodeEffectRenderLogic   ();
    virtual         ~NodeEffectRenderLogic  ();

    virtual void    RenderNode              ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic ) = 0;

protected:

    void            DrawNode                ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic );

    void            DrawNodeOnly            ( Renderer * renderer, SceneNode * node );
    void            DrawChildren            ( Renderer * renderer, SceneNode * node, int firstChildIdx, OffscreenRenderLogic * offscreenRenderLogic );

};

} //bv
