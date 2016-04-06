#pragma once

//pablito:
#include "VideoCardManager.h"
#include <vector>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"
#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"
#include "Engine/Graphics/SceneGraph/Scene.h"


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

class RenderablePass;

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

    RenderablePass *                m_boundingBoxEffect;

public:

            RenderLogic     ( unsigned int width, unsigned int height, const glm::vec4 & clearColor, bool useReadback, bool useVideoCardOutput, bool renderToSharedMemory );
            ~RenderLogic    ();

    void    RenderFrame     ( Renderer * renderer, const SceneVec & scenes );

//	//pablito
//	void	SetVideoCardManager ( bv::videocards::VideoCardManager* videoCardManager );
//private:
//    void	InitVideoCards      ();
//    // pablito end

private:


    void    RenderFrameImpl ( Renderer * renderer, const SceneVec & scenes );
    void    FrameRendered   ( Renderer * renderer );

    void    RenderRootNode  ( Renderer * renderer, const SceneVec & scenes, RenderTarget * rt );

public:

    void    RenderNode      ( SceneNode * node, RenderLogicContext * ctx );
    void    DrawNode        ( SceneNode * node, RenderLogicContext * ctx );
    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    RenderChildren  ( SceneNode * node, RenderLogicContext * ctx, int firstChildIdx = 0 );

private:

    void    RenderBoundingBox( SceneNode * node, RenderLogicContext * ctx, glm::vec4 color );
       
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
