#include "RenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "Tools/HRTimer.h"

#include "Rendering/OffscreenRenderLogic.h"
#include "BVConfig.h"

#include "BVGL.h"

#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/DefaultEffectRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/AlphaMaskRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/NodeMaskRenderLogic.h"

#define USE_HACK_FRIEND_NODE_MASK_IMPL

namespace bv {

extern HighResolutionTimer GTimer;

// *********************************
//
RenderLogic::RenderLogic     ()
{
    m_yp = CLT_TOTAL;

    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );

    m_customNodeRenderLogic.push_back( new DefaultEffectRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new AlphaMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new NodeMaskRenderLogic( this, m_offscreenRenderLogic ) );
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    for ( auto rl : m_customNodeRenderLogic )
        delete rl;

    delete m_offscreenRenderLogic;
}

// *********************************
//
void    RenderLogic::SetCamera       ( Camera * cam )
{
    m_offscreenRenderLogic->SetRendererCamera( cam );
}

// *********************************
//
//void    RenderLogic::RenderFrame     ( Renderer * renderer, SceneNode * node )
//{
//    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//    renderer->ClearBuffers();
//    renderer->PreDraw();
//
//    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
//    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//
//    renderer->ClearBuffers();
//
//
//	if( node )
//		RenderNode( renderer, node );
//
//    m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
//    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
//
//    m_offscreenRenderLogic->DrawDisplayRenderTarget( renderer );
//
//    renderer->PostDraw();
//    renderer->DisplayColorBuffer();
//}

// *********************************
//
void    RenderLogic::RenderFrameTM   ( Renderer * renderer, SceneNode * node )
{
    PreFrameSetupTM( renderer );

    // FIXME: verify that all rendering paths work as expected
	if( node )
		RenderNodeTM( renderer, node );

    PostFrameSetupTM( renderer );
}

// *********************************
//
void    RenderLogic::PreFrameSetupTM ( Renderer * renderer )
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
void    RenderLogic::PostFrameSetupTM( Renderer * renderer )
{
    m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    m_offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
//void    RenderLogic::RenderNode      ( Renderer * renderer, SceneNode * node )
//{
//    if ( node->IsVisible() )
//    {
//        if( node->IsOverridenAM() )
//        {
//            RenderAlphaMask( renderer, node );
//        }
//        else if( node->IsOverridenNM() )
//        {
//#ifndef USE_HACK_FRIEND_NODE_MASK_IMPL
//            RenderNodeMask( renderer, node );
// #else
//            RenderNodeMask1( renderer, node );
//#endif
//        }
//        else
//        {
//			RenderVanilla( renderer, node );
//        }
//    }
//}

// *********************************
//
void    RenderLogic::RenderNodeTM       ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        auto effectRenderLogic = GetNodeEffectRenderLogic( node );
        
        effectRenderLogic->RenderNode( renderer, node );
    }
}

//// *********************************
////
//bool    RenderLogic::UseDefaultMask  ( SceneNode * node ) const
//{
//    return !( UseAlphaMask( node ) || UseNodeMask( node ) );
//}
//
//// *********************************
////
//bool    RenderLogic::UseAlphaMask    ( SceneNode * node ) const
//{
//    return node->IsOverridenAM();
//}
//
//// *********************************
////
//bool    RenderLogic::UseNodeMask     ( SceneNode * node ) const
//{
//    return node->IsOverridenNM();
//}

// *********************************
//
bool    RenderLogic::UseDefaultMaskTM( SceneNode * node ) const
{
    return node->GetNodeEffect()->GetType() == NodeEffect::Type::T_DEFAULT;
}

// *********************************
//
bool    RenderLogic::UseAlphaMaskTM  ( SceneNode * node ) const
{
    return node->GetNodeEffect()->GetType() == NodeEffect::Type::T_ALPHA_MASK;
}

// *********************************
//
bool    RenderLogic::UseNodeMaskTM   ( SceneNode * node ) const
{
    return node->GetNodeEffect()->GetType() == NodeEffect::Type::T_NODE_MASK;
}

// *********************************
//
NodeEffectRenderLogic *     RenderLogic::GetNodeEffectRenderLogic    ( SceneNode * node ) const
{
    if( UseAlphaMaskTM( node ) )
    {
        return m_customNodeRenderLogic[ CLT_ALPHA_MASK ];
    }
    else if ( UseNodeMaskTM( node ) )
    {
        return m_customNodeRenderLogic[ CLT_NODE_MASK ];
    }
    else if ( UseDefaultMaskTM( node ) )
    {
        return m_customNodeRenderLogic[ CLT_DEFAULT ];
    }
    else
    {
        assert( false );
    }

    return nullptr;
}

//// *********************************
////
//void    RenderLogic::RenderVanilla   ( Renderer * renderer, SceneNode * node )
//{
//    DrawNode( renderer, node );
//}
//
//// *********************************
////
//void    RenderLogic::RenderAlphaMask ( Renderer * renderer, SceneNode * node )
//{
//    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
//    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//
//    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//    renderer->ClearBuffers();
//
//    DrawNode( renderer, node );
//
//    m_offscreenRenderLogic->DrawTopAuxRenderTarget( renderer, node->GetOverrideAlphaVal() );
//    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
//    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//}
//
//// *********************************
////
//void    RenderLogic::RenderNodeMask  ( Renderer * renderer, SceneNode * node )
//{
//    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
//    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//
//    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//    renderer->ClearBuffers();
//        
//    DrawChildren( renderer, node );
//
//    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
//    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//    renderer->ClearBuffers();
//
//    DrawNodeOnly( renderer, node );
//
//    m_offscreenRenderLogic->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
//    
//    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
//    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
//
//    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//}
//
//// *********************************
////
//void    RenderLogic::RenderNodeMask1 ( Renderer * renderer, SceneNode * node )
//{
//    if( node->NumChildNodes() < 2 )
//    {
//        RenderVanilla( renderer, node );
//    }
//    else
//    {
//        DrawNodeOnly( renderer, node );
//
//        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
//        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//
//        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//        renderer->ClearBuffers();
//        
//        // MASK
//        RenderNode( renderer, node->GetChild( 1 ) ); 
//
//        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
//        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//        renderer->ClearBuffers();
//
//        // FOREGROUND
//        RenderNode( renderer, node->GetChild( 0 ) ); 
//
//        m_offscreenRenderLogic->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
//    
//        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
//        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
//
//        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
//
//        DrawChildren( renderer, node, 2 );
//    }
//}
//
//// *********************************
////
//void    RenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node )
//{
//	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
//    DrawNodeOnly( renderer, node );
//
//    DrawChildren( renderer, node );
//}
//
//// *********************************
////
//void    RenderLogic::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
//{
//    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
//}
//
//// *********************************
////
//void    RenderLogic::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx )
//{
//    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
//    {
//        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
//        RenderNode( renderer, node->GetChild( i ) ); 
//    }
//}

// *********************************
//
void    RenderLogic::DrawNodeTM      ( Renderer * renderer, SceneNode * node )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnlyTM( renderer, node );

    DrawChildrenTM( renderer, node );
}

// *********************************
//
void    RenderLogic::DrawNodeOnlyTM  ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::DrawChildrenTM  ( Renderer * renderer, SceneNode * node, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNodeTM( renderer, node->GetChild( i ) ); 
    }
}

// *********************************
//
void    RenderLogic::FrameRendered   ( Renderer * renderer )
{
    static int w = 0;
    static int h = 0;

    if( !DefaultConfig.ReadbackFlag() )
    {
        //Not needed as it does not make sense without readback delay
        //m_offscreenRenderLogic->SwapDisplayRenderTargets();
        return;
    }

    if( w != renderer->GetWidth() || h != renderer->GetHeight() )
    {
        w = renderer->GetWidth();
        h = renderer->GetHeight();

        printf( "Framebuffer resolution changed to %dx%d\n", w, h );
    }

    static double totalElapsed = 0.0;
    static int nFrames = 1;
    static int nPasses = 0;
    static int nReadbackFrame = 0;

    double readbackStart = GTimer.CurElapsed();
    auto frame = m_offscreenRenderLogic->ReadDisplayTarget( renderer, nReadbackFrame );
    nReadbackFrame = ( nReadbackFrame + 1 ) % m_offscreenRenderLogic->NumReadBuffersPerRT();
    double readbackTime = GTimer.CurElapsed() - readbackStart;

    m_offscreenRenderLogic->SwapDisplayRenderTargets();

    totalElapsed += readbackTime;

    //printf( "Time cur %.5f ms of total %.5f ms\n", 1000.f * readbackTime, 1000.f * totalElapsed );

    if( nFrames % 50 == 0 )
    {
        //double avg = totalElapsed / (double) nFrames;

        nPasses++;
        totalElapsed = 0.0;

        if ( nPasses % 3 == 0 )
        {
            nPasses = 0;

            //printf( "Avg readback time from last %d frames took %.4f ms\n", nFrames, avg * 1000 );
        }

        nFrames = 0;
    }

    nFrames++;
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
