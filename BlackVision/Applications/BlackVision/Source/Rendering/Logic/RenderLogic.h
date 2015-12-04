#pragma once

//pablito:
#include "VideoCardManager.h"
#include <vector>


namespace bv {

class Renderer;
class OffscreenRenderLogic;
class SceneNode;
class Camera;
class NodeEffectRenderLogic;
class DefaultVideoOutputRenderLogic;
class ScreenShotLogic;

class RenderLogic
{
private:

	bv::videocards::VideoCardManager *      m_VideoCardManager;

    OffscreenRenderLogic *                  m_offscreenRenderLogic;
    DefaultVideoOutputRenderLogic *         m_videoOutputRenderLogic;
    ScreenShotLogic*                        m_screenShotLogic;

    std::vector< NodeEffectRenderLogic * >  m_customNodeRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );
    void    PreFrameSetup   ( Renderer * renderer );
    void    PostFrameSetup  ( Renderer * renderer );
	//pablito
	void	SetVideoCardManager(bv::videocards::VideoCardManager* videoCardManager, Renderer * renderer);
	void	InitVideoCards     ( Renderer * renderer );

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

public:
    void    MakeScreenShot  ( const std::string& path, unsigned int numFrames );
};

} // bv
