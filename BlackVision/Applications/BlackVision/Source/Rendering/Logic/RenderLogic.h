#pragma once

//pablito:
#include "VideoCardManager.h"
#include <vector>

#include "Rendering/Utils/RenderTargetStackAllocator.h"
#include "Rendering/Logic/FrameRendering/NodeEffectRenderLogicSelector.h"
#include "Rendering/SharedMemoryVideoBuffer.h"

namespace bv {

class Renderer;
class SceneNode;
class Camera;
class NodeEffectRenderLogic;
class DefaultVideoOutputRenderLogic;
class ScreenShotLogic;

class RenderTarget;

class PostFrameRenderLogic;
class OffscreenDisplay;
class BlitFullscreenEffect;
class VideoOutputRenderLogic;
class RenderLogicContext;

class RenderLogic
{
private:

	bv::videocards::VideoCardManager *      m_VideoCardManager;
    RenderTargetStackAllocator      m_rtStackAllocator;
    NodeEffectRenderLogicSelector   m_nodeEffectRenderLogicSelector;
    OffscreenDisplay *              m_offscreenDisplay;
    BlitFullscreenEffect *          m_blitEffect;
    VideoOutputRenderLogic *        m_videoOutputRenderLogic;

    ScreenShotLogic*                        m_screenShotLogic;

    bool                            m_displayVideoCardPreview;
    bool                            m_useVideoCardOutput;
	SharedMemoryVideoBuffer*		m_SharedMemoryVideoBuffer;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    RenderFrame     ( Renderer * renderer, SceneNode * sceneRoot );

	//pablito
	void	SetVideoCardManager ( bv::videocards::VideoCardManager* videoCardManager );
private:
    void	InitVideoCards      ();
    // pablito end

private:


    void    RenderFrameImpl ( Renderer * renderer, SceneNode * sceneRoot );
    void    FrameRendered   ( Renderer * renderer );

    void    RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt );
       
public:

    void    RenderNode      ( Renderer * renderer, SceneNode * node, RenderLogicContext * ctx );
    void    DrawNode        ( Renderer * renderer, SceneNode * node, RenderLogicContext * ctx );
    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    RenderChildren  ( Renderer * renderer, SceneNode * node, RenderLogicContext * ctx, int firstChildIdx = 0 );

private:

    BlitFullscreenEffect *          AccessBlitEffect        ( RenderTarget * rt );
    void                            BlitToPreview           ( Renderer * renderer, RenderTarget * rt );
    void                            PushToVideoCard         ( Texture2DConstPtr frame );

    void                            UpdateOffscreenState    ();
    void                            OnVideoFrameRendered    ( Renderer * renderer );

public:

    VideoOutputRenderLogic *        GedVideoOutputRenderLogic   ();

public:
    void    MakeScreenShot  ( const std::string& path, unsigned int numFrames );
};

} // bv
