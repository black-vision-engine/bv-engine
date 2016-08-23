#pragma once

//pablito:
#include "VideoCardManager.h"
#include <vector>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Utils/RenderQueueStackAllocator.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"
#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"
#include "Engine/Graphics/SceneGraph/Scene.h"

#include "Mathematics/glm_inc.h"


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

namespace audio {
    class AudioRenderer;
}

class RenderLogic
{
private:

    videocards::VideoCardManager *  m_VideoCardManager;
    RenderTargetStackAllocator      m_rtStackAllocator;
    RenderQueueStackAllocator       m_renderQueueAllocator;
    OffscreenDisplay *              m_offscreenDisplay;
    FullscreenEffectInstance *      m_blitEffect;
    VideoOutputRenderLogic *        m_videoOutputRenderLogic;
    RenderLogicContext *            m_ctx;

    ScreenShotLogic*                m_screenShotLogic;

    bool                            m_displayVideoCardPreview;
    bool                            m_useVideoCardOutput;
    SharedMemoryVideoBuffer*        m_SharedMemoryVideoBuffer;

    glm::vec4                       m_clearColor;
    bool                            m_renderToSharedMemory;

    RenderablePass *                m_boundingBoxEffect;

public:

            RenderLogic     ( unsigned int width, unsigned int height, const glm::vec4 & clearColor, bool useReadback, bool useVideoCardOutput, bool renderToSharedMemory );
            ~RenderLogic    ();

    void    RenderFrame     ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes );

private:

    void    RenderFrameImpl ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes );
    void    FrameRendered   ( Renderer * renderer );

    void    RenderRootNode  ( Renderer * renderer, const SceneVec & scenes, RenderTarget * rt );

    // ------ audio ------
    void    RenderRootNode  ( audio::AudioRenderer * audioRenderer, const SceneVec & scenes );

public:

    void    RenderNode      ( SceneNode * node, RenderLogicContext * ctx );
    void    DrawNode        ( SceneNode * node, RenderLogicContext * ctx );
    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    RenderChildren  ( SceneNode * node, RenderLogicContext * ctx, int firstChildIdx = 0 );

    // ------ audio ------
    void    Play            ( audio::AudioRenderer * audioRenderer, SceneNode * node );

    // Moved from private for RenderingQueue logic
    void    RenderBoundingBox   ( SceneNode * node, RenderLogicContext * ctx, glm::vec4 color );

private:

    void    RenderGridLines     ( Scene * scene, RenderLogicContext * ctx );
       
    RenderLogicContext *            GetContext              ( Renderer * renderer );

    void                            PushToVideoCard         ( Texture2DConstPtr frame );
    void                            BlitToPreview           ( RenderTarget * rt, RenderLogicContext * ctx );

    void                            UpdateOffscreenState    ();
    void                            OnVideoFrameRendered    ( RenderLogicContext * ctx );

public:

    VideoOutputRenderLogic *        GedVideoOutputRenderLogic   ();

public:
    void    MakeScreenShot  ( const std::string& path, unsigned int numFrames, bool onRenderedEvent, bool asyncWrite = true );
};

} // bv
