#include "RenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Rendering/OffscreenRenderLogic.h"
#include "BVConfig.h"

#include "BVGL.h"

#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/DefaultEffectRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/AlphaMaskRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/NodeMaskRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/WireframeRenderLogic.h"

#include "Rendering/Logic/VideoOutputRendering/DefaultVideoOutputRenderLogic.h"


namespace bv {

// *********************************
//
RenderLogic::RenderLogic     ()
{
    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );
    m_videoOutputRenderLogic = new DefaultVideoOutputRenderLogic( DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput() );

    m_customNodeRenderLogic.push_back( new DefaultEffectRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new AlphaMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new NodeMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new WireframeRenderLogic( this, m_offscreenRenderLogic ) );
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    for ( auto rl : m_customNodeRenderLogic )
        delete rl;

    delete m_offscreenRenderLogic;
    delete m_videoOutputRenderLogic;
}

// *********************************
//
void    RenderLogic::SetCamera       ( Camera * cam )
{
    m_offscreenRenderLogic->SetRendererCamera( cam );
}

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * node )
{
    PreFrameSetup( renderer );

    // FIXME: verify that all rendering paths work as expected
	if( node )
		RenderNode( renderer, node );

    PostFrameSetup( renderer );
}

// *********************************
//
void    RenderLogic::PreFrameSetup  ( Renderer * renderer )
{
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
    renderer->PreDraw();

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->ClearBuffers();
}

// *********************************
//
void    RenderLogic::PostFrameSetup ( Renderer * renderer )
{
    m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    RenderLogic::RenderNode     ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        auto effectRenderLogic = GetNodeEffectRenderLogic( node );
        
        effectRenderLogic->RenderNode( renderer, node );
    }
}

// *********************************
//
NodeEffectRenderLogic *     RenderLogic::GetNodeEffectRenderLogic    ( SceneNode * node ) const
{
    assert( (unsigned int) node->GetNodeEffect()->GetType() < (unsigned int) NodeEffect::Type::T_TOTAL );

    return m_customNodeRenderLogic[ (unsigned int) node->GetNodeEffect()->GetType() ];
}

// *********************************
//
void    RenderLogic::DrawNode       ( Renderer * renderer, SceneNode * node )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer, node );

    DrawChildren( renderer, node );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( renderer, node->GetChild( i ) ); 
    }
}

// *********************************
//
void    RenderLogic::PrintGLStats    (  bool detailed  )
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
