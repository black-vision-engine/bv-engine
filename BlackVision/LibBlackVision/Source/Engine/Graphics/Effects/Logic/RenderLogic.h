#pragma once

#include "VideoCardManager.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"
#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Utils/RenderQueueStackAllocator.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"
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

// MARK REMOVE
class RenderLogic
{
private:

    RenderTargetStackAllocator      m_rtStackAllocator;
    RenderQueueStackAllocator       m_renderQueueAllocator;
    OffscreenDisplay *              m_offscreenDisplay;
    FullscreenEffectInstance *      m_blitEffect;
    VideoOutputRenderLogic *        m_videoOutputRenderLogic;
    RenderLogicContext *            m_ctx;

    bool                            m_displayVideoCardPreview;
    bool                            m_useVideoCardOutput;

    glm::vec4                       m_clearColor;

    RenderablePass *                m_boundingBoxEffect;

    // FIXME: nrl - add shared memory to nrl pipeline
	SharedMemoryVideoBuffer *       m_sharedMemoryVideoBuffer;
	int						        m_sharedMemoryScaleFactor;
    bool                            m_enableSharedMemory;

public:

            RenderLogic     ( unsigned int width, unsigned int height, const glm::vec4 & clearColor, bool useReadback, bool useVideoCardOutput, bool enableSharedMemory, int scaleFactor );
            ~RenderLogic    ();

    void    RenderFrame     ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes );

private:

    void    RenderFrameImpl ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes );
    void    FrameRendered   ( Renderer * renderer, audio::AudioRenderer * audioRenderer );

    void    RenderRootNode  ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes, RenderTarget * rt );

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
       
    RenderLogicContext *            GetContext              ( Renderer * renderer, audio::AudioRenderer * audioRenderer );

    void                            BlitToPreview           ( RenderTarget * rt, RenderLogicContext * ctx );

    void                            UpdateOffscreenState    ();
    void                            OnVideoFrameRendered    ( RenderLogicContext * ctx );

public:

    VideoOutputRenderLogic *        GedVideoOutputRenderLogic   ();

};

} // bv
