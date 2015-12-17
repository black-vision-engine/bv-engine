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

#include "Rendering/Logic/FullScreen/Impl/BlitFullscreenEffect.h"

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
    , m_blitEffect( nullptr )
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
    delete m_blitEffect;
}

// *********************************
//
void    RenderLogic::SetCamera       ( Camera * cam )
{
    m_frameRenderLogic->SetCamera( cam );
    
    // m_offscreenRenderLogic->SetRendererCamera( cam );
}

#define USE_NEW_RENDER_LOGIC
#define USE_DEFAULT_EFFECT_ONLY
//#define USE_DEFAULT_AND_ALPHA_EFFECTS_ONLY

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    renderer->PreDraw();

#ifndef USE_NEW_RENDER_LOGIC
    m_frameRenderLogic->RenderFrame( renderer, sceneRoot );
#else
    NewRenderFrame( renderer, sceneRoot );
#endif

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    RenderLogic::NewRenderFrame  ( Renderer * renderer, SceneNode * sceneRoot )
{
    auto rt = m_offscreenDisplay->GetActiveRenderTarget();

    RenderRootNode( renderer, sceneRoot, rt );
    BlitToPreview( renderer, rt );

    UpdateOffscreenState();
}

// *********************************
//
void    RenderLogic::RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt )
{
    // FIXME: verify that all rendering paths work as expected
	if( sceneRoot )
    {
        renderer->Enable( rt );

        {
            renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
            renderer->ClearBuffers();

            RenderNode( renderer, sceneRoot );
        }

        renderer->Disable( rt );
    }
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
        #ifdef USE_DEFAULT_EFFECT_ONLY
            // Default render logic
            DrawNode( renderer, node );
        #elif defined(USE_DEFAULT_AND_ALPHA_EFFECTS_ONLY)
            if( node->GetNodeEffect()->GetType() == NodeEffect::Type::T_DEFAULT || node->GetNodeEffect()->GetType() != NodeEffect::Type::T_ALPHA_MASK )
            {
                // Default render logic
                DrawNode( renderer, node );
            }
            else
            {
                auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogicTr( node );
               
                effectRenderLogic->RenderNode( node, &ctx );
            }
        #else
            if( node->GetNodeEffect()->GetType() == NodeEffect::Type::T_DEFAULT )
            {
                // Default render logic
                DrawNode( renderer, node );
            }
            else
            {
                auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogicTr( node );
               
                effectRenderLogic->RenderNode( node, &ctx );
            }
        #endif
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

// *********************************
//
BlitFullscreenEffect *  RenderLogic::AccessBlitEffect   ( RenderTarget * rt )
{
    if ( !m_blitEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );

        m_blitEffect = new BlitFullscreenEffect( rtTex, false );
    }

    return m_blitEffect;
}

// *********************************
//
void                    RenderLogic::BlitToPreview      ( Renderer * renderer, RenderTarget * rt )
{
    assert( rt == m_offscreenDisplay->GetActiveRenderTarget() );

    // Blit current render target - suxx a bit - there should be a separate initialization step
    // Render fullscreen effect
    auto blitter = AccessBlitEffect( rt );

    blitter->Render( renderer );
}

// *********************************
//
void                    RenderLogic::UpdateOffscreenState()
{
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();
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