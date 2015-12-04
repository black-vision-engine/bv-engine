#pragma once

namespace bv {

class RenderLogicContext;
class OffscreenRenderLogic;
class SceneNode;
class FrameRenderLogic;

class NodeEffectRenderLogicTr
{
public:

                    NodeEffectRenderLogicTr     ();
    virtual         ~NodeEffectRenderLogicTr    ();

    virtual void    RenderNode                  ( SceneNode * node, RenderLogicContext * ctx ) = 0;

};

} //bv
