#include "RenderLogic.h"

#include "Rendering/Logic/RenderLogicImpl/RenderLogicRawPreview.h"
#include "Rendering/Logic/RenderLogicImpl/RenderLogicVideoPreview.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Logic/FrameRendering/FrameRenderLogic.h"
#include "Rendering/Logic/FrameRendering/PostFrameRenderLogic.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Rendering/Utils/OffscreenRenderLogic.h"
#include "BVConfig.h"

#include "BVGL.h"

#include "Rendering/Logic/VideoOutputRendering/DefaultVideoOutputRenderLogic.h"
#include "BVConfig.h"


namespace bv {

// *********************************
//
RenderLogic::RenderLogic     ()
    :  m_impl( nullptr )
    , m_rtStack( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), TextureFormat::F_A8R8G8B8 )
{
    auto videoCardEnabled   = DefaultConfig.ReadbackFlag();
    auto previewAsVideoCard = DefaultConfig.DisplayVideoCardOutput();
   
    if( previewAsVideoCard )
    {
        m_impl = new RenderLogicVideoPreview( videoCardEnabled, &m_rtStack );
    }
    else
    {
        m_impl = new RenderLogicRawPreview( videoCardEnabled, &m_rtStack );
    }

    m_frameRenderLogic = new FrameRenderLogic();
    m_postFrameRenderLogic = new PostFrameRenderLogic();
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    delete m_impl;
    delete m_frameRenderLogic;
    delete m_postFrameRenderLogic;
}

// *********************************
//
void    RenderLogic::SetCamera       ( Camera * cam )
{
    m_frameRenderLogic->SetCamera( cam );
}

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    m_frameRenderLogic->RenderFrame( renderer, sceneRoot );

    // m_impl->RenderFrame( renderer, sceneRoot );
}

} //bv


/* 

Perfect world:

Rendering -> Frame rendered to texture -> POST RENDER DISPLAY LOGIC

POST RENDER DISPLAY LOGIC: (option a: display to the preview) -> stack current frame -> apply per pixel effect -> interlace with previous frame -> (option b: display to the preview) | display to video card


Not so ideal world:

Rendering -> render logic (with swapping render targets) -> some code dependent on render logic and render targets -> bookkeeping of rendered frames -> not so well isolated POST RENDER DISPLAY LOGIC

POST RENDER DISPLAY LOGIC: (option a: display to the preview) -> somehow use render logic and offscreen render logic to interlace last two frames -> apply pixel shaders afterwards -> (option b: display to the preview) | display to video card

What makes it even more cumbersome is that RenderLogic is used by global effect render logic inplementation whereas a separated class created solely for this purpose should be used
RenderLogic should only serve as a dispatcher of rendering tasks to appropriate render logic implementations
OffscreenRenderLogic should also be split in two - blending logic and other logic AND a separate class used only to manipulate render targets

*/