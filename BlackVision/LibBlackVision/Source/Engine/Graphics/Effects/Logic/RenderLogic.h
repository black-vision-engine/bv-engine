#pragma once

#include <vector>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"


namespace bv {

class Renderer;
class SceneNode;

class RenderTarget;

class PostFrameRenderLogic;
class OffscreenDisplay;
class BlitFullscreenEffect;
class VideoOutputRenderLogic;
class RenderLogicContext;
class SimpleFullscreenEffect;
class FullscreenEffectInstance;

class RenderLogic
{
private:

    RenderTargetStackAllocator      m_rtStackAllocator;
    OffscreenDisplay *              m_offscreenDisplay;
    BlitFullscreenEffect *          m_blitEffect;
    VideoOutputRenderLogic *        m_videoOutputRenderLogic;
        
    //FIXME: implement wrapper class which uses al three to implement an effect (thin accessors wrapper)
    FullscreenEffectInstance *      m_blitEffectTr;
    //FullscreenEffectContext         m_blitCtx;
    std::vector< RenderTarget * >   m_blitInputRt;

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
    FullscreenEffectInstance *      AccessBlitEffectTr      ( Renderer * renderer, RenderTargetStackAllocator * allocator, RenderTarget * rt );

    void                            BlitToPreview           ( Renderer * renderer, RenderTarget * rt );
    void                            BlitToPreviewTr         ( Renderer * renderer, RenderTarget * rt );

    void                            UpdateOffscreenState    ();
    void                            OnVideoFrameRendered    ( Renderer * renderer );

public:

    VideoOutputRenderLogic *        GedVideoOutputRenderLogic   ();

};

} // bv
