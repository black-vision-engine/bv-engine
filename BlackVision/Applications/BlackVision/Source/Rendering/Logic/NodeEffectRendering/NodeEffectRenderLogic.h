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

    virtual void    Render                  ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic ) = 0;

};

} //bv
