#pragma once

//pablito:
#include "VideoCardManager.h"
#include <vector>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"
#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"

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
class FullscreenEffectInstance;

class RenderLogic
{
private:

	bv::videocards::VideoCardManager *      m_VideoCardManager;
    RenderTargetStackAllocator      m_rtStackAllocator;
    OffscreenDisplay *              m_offscreenDisplay;
    FullscreenEffectInstance *      m_blitEffect;
    VideoOutputRenderLogic *        m_videoOutputRenderLogic;
    RenderLogicContext *            m_ctx;

    ScreenShotLogic*                m_screenShotLogic;

    bool                            m_displayVideoCardPreview;
    bool                            m_useVideoCardOutput;
	SharedMemoryVideoBuffer*		m_SharedMemoryVideoBuffer;

    glm::vec4                       m_clearColor;
    bool                            m_renderToSharedMemory;

public:

            RenderLogic     ( unsigned int width, unsigned int height, const glm::vec4 & clearColor, bool useReadback, bool useVideoCardOutput, bool renderToSharedMemory );
            ~RenderLogic    ();

    void    RenderFrame     ( Renderer * renderer, SceneNode * sceneRoot );

//	//pablito
//	void	SetVideoCardManager ( bv::videocards::VideoCardManager* videoCardManager );
//private:
//    void	InitVideoCards      ();
//    // pablito end

private:


    void    RenderFrameImpl ( Renderer * renderer, SceneNode * sceneRoot );
    void    FrameRendered   ( Renderer * renderer );

    void    RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt );
       
public:

    void    RenderNode      ( SceneNode * node, RenderLogicContext * ctx );
    void    DrawNode        ( SceneNode * node, RenderLogicContext * ctx );
    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    RenderChildren  ( SceneNode * node, RenderLogicContext * ctx, int firstChildIdx = 0 );

private:

    RenderLogicContext *            GetContext              ( Renderer * renderer );

    //void                            PushToVideoCard         ( Texture2DConstPtr frame );
    void                            BlitToPreview           ( RenderTarget * rt, RenderLogicContext * ctx );

    void                            UpdateOffscreenState    ();
    void                            OnVideoFrameRendered    ( RenderLogicContext * ctx );

public:

    VideoOutputRenderLogic *        GedVideoOutputRenderLogic   ();

public:
    void    MakeScreenShot  ( const std::string& path, unsigned int numFrames, bool onRenderedEvent, bool asyncWrite = true );
};

} // bv
