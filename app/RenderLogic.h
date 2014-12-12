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
    
    void    RenderVanilla   ( Renderer * renderer, SceneNode * node );
    void    RenderAlphaMask ( Renderer * renderer, SceneNode * node );
    void    RenderNodeMask  ( Renderer * renderer, SceneNode * node );

    void    DrawNode        ( Renderer * renderer, SceneNode * node );

    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    DrawChildren    ( Renderer * renderer, SceneNode * ndoe );

public:

    void    FrameRendered   ( Renderer * renderer );

};

} // bv
