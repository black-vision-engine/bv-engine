#include "RenderLogic.h"

#include "Rendering/Logic/RenderLogicImpl/RenderLogicRawPreview.h"
#include "Rendering/Logic/RenderLogicImpl/RenderLogicVideoPreview.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogicTr.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Utils/OffscreenDisplay.h"

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
    : m_impl( nullptr )
    , m_rtStackAllocator( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), TextureFormat::F_A8R8G8B8 )
{
    auto videoCardEnabled   = DefaultConfig.ReadbackFlag();
    auto previewAsVideoCard = DefaultConfig.DisplayVideoCardOutput();

    m_offscreenDisplay = new OffscreenDisplay( &m_rtStackAllocator, videoCardEnabled || previewAsVideoCard );

    m_impl = new RenderLogicImpl( &m_rtStackAllocator, videoCardEnabled || previewAsVideoCard );
    m_frameRenderLogic = new FrameRenderLogic();
    m_postFrameRenderLogic = new PostFrameRenderLogic();
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    delete m_offscreenDisplay;
    delete m_impl;
    delete m_frameRenderLogic;
    delete m_postFrameRenderLogic;
}

// *********************************
//
void    RenderLogic::SetCamera       ( Camera * cam )
{
    m_frameRenderLogic->SetCamera( cam );
    
    // m_offscreenRenderLogic->SetRendererCamera( cam );
}

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    m_frameRenderLogic->RenderFrame( renderer, sceneRoot );

#if 0
    // Pre frame setup
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
    renderer->PreDraw();
    
    renderer->Enable( m_offscreenDisplay->GetActiveRenderTarget() );

    // FIXME: verify that all rendering paths work as expected
	if( sceneRoot )
		RenderNode( renderer, sceneRoot );

    // Post frame logic
    renderer->Disable( m_offscreenDisplay->GetActiveRenderTarget() );
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();

    //m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    //m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    //m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
#endif
}

// *********************************
//
void    RenderLogic::RenderNode      ( Renderer * renderer, SceneNode * node )
{
    //FIXME: assumes only one renderer instance per application
    static RenderLogicContext ctx( renderer, &m_rtStackAllocator, this );

    assert( renderer == ctx.GetRenderer() );

    if ( node->IsVisible() )
    {
        auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogicTr( node );
                
        effectRenderLogic->RenderNode( node, &ctx );
    }
}

// *********************************
//
void    RenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer, node );

    RenderChildren( renderer, node );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly    ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::RenderChildren  ( Renderer * renderer, SceneNode * node, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( renderer, node->GetChild( i ) ); 
    }
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