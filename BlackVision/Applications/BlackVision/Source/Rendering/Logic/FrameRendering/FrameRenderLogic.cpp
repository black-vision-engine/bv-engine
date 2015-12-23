#include "FrameRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Rendering/Utils/OffscreenRenderLogic.h"
#include "BVConfig.h"

#include "BVGL.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

#include "Rendering/Logic/VideoOutputRendering/VideoOutputRenderLogic.h"


namespace bv {

// *********************************
//
FrameRenderLogic::FrameRenderLogic     ()
{
    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );
    m_videoOutputRenderLogic = new VideoOutputRenderLogic( DefaultConfig.DefaultHeight() );
    // DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput() 
/*
    m_customNodeRenderLogic.push_back( new DefaultEffectRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new AlphaMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new NodeMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new WireframeRenderLogic( this, m_offscreenRenderLogic ) );
*/
}

// *********************************
//
FrameRenderLogic::~FrameRenderLogic    ()
{
    for ( auto rl : m_customNodeRenderLogic )
        delete rl;

    delete m_offscreenRenderLogic;
    delete m_videoOutputRenderLogic;
}

// *********************************
//
void    FrameRenderLogic::SetCamera       ( Camera * cam )
{
    m_offscreenRenderLogic->SetRendererCamera( cam );
}

// *********************************
//
void    FrameRenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    PreFrameSetup( renderer );

    // FIXME: verify that all rendering paths work as expected
	if( sceneRoot )
		RenderNode( renderer, sceneRoot );

    PostFrameSetup( renderer );
}

// *********************************
//
void    FrameRenderLogic::PreFrameSetup  ( Renderer * renderer )
{
//FIXME: it is a fullscreen render pass, so no such things are required now
//    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//    renderer->ClearBuffers();
    renderer->PreDraw();

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->ClearBuffers();
}

// *********************************
//
void    FrameRenderLogic::PostFrameSetup ( Renderer * renderer )
{
    m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    // m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    FrameRenderLogic::RenderNode     ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        auto effectRenderLogic = GetNodeEffectRenderLogic( node );
        
        { renderer; effectRenderLogic; }
        //effectRenderLogic->RenderNode( renderer, node );
    }
}

// *********************************
//
NodeEffectRenderLogic *     FrameRenderLogic::GetNodeEffectRenderLogic    ( SceneNode * node ) const
{
    assert( (unsigned int) node->GetNodeEffect()->GetType() < (unsigned int) NodeEffect::Type::T_TOTAL );

    return m_customNodeRenderLogic[ (unsigned int) node->GetNodeEffect()->GetType() ];
}

// *********************************
//
void    FrameRenderLogic::DrawNode       ( Renderer * renderer, SceneNode * node )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer, node );

    DrawChildren( renderer, node );
}

// *********************************
//
void    FrameRenderLogic::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    FrameRenderLogic::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( renderer, node->GetChild( i ) ); 
    }
}

// *********************************
//
void    FrameRenderLogic::PrintGLStats    (  bool detailed  )
{
    if ( detailed )
    {
        BVGL::PrintCompleteSummary( " ************************* DETAILED GL STATS ********************************** " );
    }
    else
    {
        BVGL::PrintShortSummary( " ************************* SHORT GL STATS ********************************** " );
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

What makes it even more cumbersome is that FrameRenderLogic is used by global effect render logic inplementation whereas a separated class created solely for this purpose should be used
FrameRenderLogic should only serve as a dispatcher of rendering tasks to appropriate render logic implementations
OffscreenRenderLogic should also be split in two - blending logic and other logic AND a separate class used only to manipulate render targets

*/