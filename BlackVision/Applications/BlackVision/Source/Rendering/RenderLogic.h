#pragma once

//pablito:
#include "VideoCardManager.h"

namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class Camera;

class RenderLogic
{
private:

    OffscreenRenderLogic *  m_offscreenRenderLogic;
	bv::videocards::VideoCardManager *      m_VideoCardManager;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );

	//pablito
	void	SetVideoCardManager(bv::videocards::VideoCardManager* videoCardManager, Renderer * renderer);
	void	InitVideoCards     ( Renderer * renderer );

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
