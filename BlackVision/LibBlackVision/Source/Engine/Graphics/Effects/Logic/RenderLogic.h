#pragma once

#include <vector>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffectRenderLogicSelector.h"


namespace bv {

class Renderer;
class SceneNode;

class RenderTarget;

class PostFrameRenderLogic;
class OffscreenDisplay;
class BlitFullscreenEffect;
class VideoOutputRenderLogic;
class RenderLogicContext;

class RenderLogic
{
private:

    RenderTargetStackAllocator      m_rtStackAllocator;
    NodeEffectRenderLogicSelector   m_nodeEffectRenderLogicSelector;
    OffscreenDisplay *              m_offscreenDisplay;
    BlitFullscreenEffect *          m_blitEffect;
    VideoOutputRenderLogic *        m_videoOutputRenderLogic;

    bool                            m_displayVideoCardPreview;
    bool                            m_useVideoCardOutput;

public:

            RenderLogic     ( unsigned int width, unsigned int height, bool useReadback, bool useVideoCardOutput );
            ~RenderLogic    ();

    void    RenderFrame     ( Renderer * renderer, SceneNode * sceneRoot );

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

    BlitFullscreenEffect *          AccessBlitEffect        ( RenderTarget * rt );
    void                            BlitToPreview           ( Renderer * renderer, RenderTarget * rt );

    void                            UpdateOffscreenState    ();
    void                            OnVideoFrameRendered    ( Renderer * renderer );

public:

    VideoOutputRenderLogic *        GedVideoOutputRenderLogic   ();

};

} // bv