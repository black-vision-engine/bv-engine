#pragma once
/**@file RenderLogic.h
Local copy of file from project BlackVision.

Class member m_offscreenRenderLogic changed to protected.*/



namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class Camera;

class RenderLogic
{
protected:

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
    void    RenderNodeMask1 ( Renderer * renderer, SceneNode * node );

    void    DrawNode        ( Renderer * renderer, SceneNode * node );

    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    DrawChildren    ( Renderer * renderer, SceneNode * node, int firstChildIdx = 0 );

public:

    void    FrameRendered   ( Renderer * renderer );
    void    PrintGLStats    ( bool detailed );

};

} // bv
