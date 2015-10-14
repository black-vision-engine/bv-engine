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

// Temporary transition code START
    void    RenderFrameTM   ( Renderer * renderer, SceneNode * node );
    void    PreFrameSetupTM ( Renderer * renderer );
    void    PostFrameSetupTM( Renderer * renderer );
// Temporary transition code END

private:

    void    RenderNode      ( Renderer * renderer, SceneNode * node );

// Temporary transition code START
    void    RenderNodeTM    ( Renderer * renderer, SceneNode * node );
    bool    UseDefaultMask  ( SceneNode * node ) const;
    bool    UseAlphaMask    ( SceneNode * node ) const;
    bool    UseNodeMask     ( SceneNode * node ) const;
// Temporary transition code END

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
