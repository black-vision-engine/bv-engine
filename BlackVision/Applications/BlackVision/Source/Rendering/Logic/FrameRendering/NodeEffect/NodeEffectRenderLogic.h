#pragma once

namespace bv {

class RenderLogicContext;
class SceneNode;

class NodeEffectRenderLogic
{
public:

                    NodeEffectRenderLogic       ();
    virtual         ~NodeEffectRenderLogic      ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) = 0;

};

} //bv
