#pragma once

#include <vector>


namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class Camera;
class NodeEffectRenderLogic;
class DefaultVideoOutputRenderLogic;

class RenderLogic
{
private:

    OffscreenRenderLogic *                  m_offscreenRenderLogic;
    DefaultVideoOutputRenderLogic *         m_videoOutputRenderLogic;

    std::vector< NodeEffectRenderLogic * >  m_customNodeRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );
    void    PreFrameSetup   ( Renderer * renderer );
    void    PostFrameSetup  ( Renderer * renderer );

// FIXME: this interface should be private or moved to another service class used by RenderLogic and Effects
public:

    void    RenderNode      ( Renderer * renderer, SceneNode * node );

private:

    NodeEffectRenderLogic *     GetNodeEffectRenderLogic    ( SceneNode * node ) const;

public:

    void    DrawNode        ( Renderer * renderer, SceneNode * node );

    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    DrawChildren    ( Renderer * renderer, SceneNode * node, int firstChildIdx = 0 );

private:

    void    PrintGLStats    ( bool detailed );

};

} // bv
