#pragma once

#include <vector>

#include "Rendering/Utils/RenderTargetStackAllocator.h"



namespace bv {

class Camera;
class Renderer;
class SceneNode;

class RenderLogicImpl;
class FrameRenderLogic;
class PostFrameRenderLogic;

class RenderLogic
{
private:

    RenderTargetStackAllocator  m_rtStackAllocator;

    RenderLogicImpl *           m_impl;
    FrameRenderLogic *          m_frameRenderLogic;
    PostFrameRenderLogic *      m_postFrameRenderLogic;

public:

            RenderLogic     ();
            ~RenderLogic    ();

    void    SetCamera       ( Camera * cam );

    void    RenderFrame     ( Renderer * renderer, SceneNode * node );

};

} // bv
