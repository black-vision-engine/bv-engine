#pragma once

namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class Camera;

class RenderLogic
{
private:

    OffscreenRenderLogic *  m_offscreenRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );

private:

    void    RenderNode      ( Renderer * renderer, SceneNode * node );
    void    DrawNode        ( Renderer * renderer, SceneNode * node );

public:

    void    FrameRendered   ( Renderer * renderer );

};

} // bv
