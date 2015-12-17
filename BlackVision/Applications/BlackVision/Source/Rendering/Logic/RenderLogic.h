#pragma once

#include <vector>

#include "Rendering/Utils/RenderTargetStackAllocator.h"
#include "Rendering/Logic/FrameRendering/NodeEffectRenderLogicSelector.h"



namespace bv {

class Camera;
class Renderer;
class SceneNode;

class RenderTarget;

class RenderLogicImpl;
class FrameRenderLogic;
class PostFrameRenderLogic;
class OffscreenDisplay;
class BlitFullscreenEffect;

class RenderLogic
{
private:

    RenderTargetStackAllocator      m_rtStackAllocator;
    NodeEffectRenderLogicSelector   m_nodeEffectRenderLogicSelector;
    OffscreenDisplay *              m_offscreenDisplay;
    BlitFullscreenEffect *          m_blitEffect;

    RenderLogicImpl *           m_impl;
    FrameRenderLogic *          m_frameRenderLogic;
    PostFrameRenderLogic *      m_postFrameRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * sceneRoot );

    void    NewRenderFrame  ( Renderer * renderer, SceneNode * sceneRoot );

private:

    void    RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt );

public:


    void    RenderNode      ( Renderer * renderer, SceneNode * node );
    void    DrawNode        ( Renderer * renderer, SceneNode * node );
    void    DrawNodeOnly    ( Renderer * renderer, SceneNode * node );
    void    RenderChildren  ( Renderer * renderer, SceneNode * node, int firstChildIdx = 0 );

private:

    BlitFullscreenEffect *  AccessBlitEffect    ( RenderTarget * rt );

    void                    BlitToPreview       ( Renderer * renderer, RenderTarget * rt );

    void                    UpdateOffscreenState();
};

} // bv
