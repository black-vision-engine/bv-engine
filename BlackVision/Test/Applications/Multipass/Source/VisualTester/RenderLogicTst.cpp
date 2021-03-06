//#include "RenderLogicTst.h"
//
//#include "Engine/Graphics/Renderers/Renderer.h"
//#include "Engine/Graphics/SceneGraph/SceneNode.h"
//#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
//
//#include "Tools/Profiler/HerarchicalProfiler.h"
//#include "Tools/HRTimer.h"
//
//#include "BVConfig.h"
//
//#include "BVGL.h"
//
//
//#define USE_HACK_FRIEND_NODE_MASK_IMPL
//
//namespace bv {
//
//extern HighResolutionTimer GTimer;
//
//// *********************************
////
//RenderLogicTst::RenderLogicTst     ()
//{
//    m_offscreenRenderLogicTst = new OffscreenRenderLogicTst( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );
//}
//
//// *********************************
////
//RenderLogicTst::~RenderLogicTst    ()
//{
//    delete m_offscreenRenderLogicTst;
//}
//
//// *********************************
////
//void    RenderLogicTst::SetCamera       ( Camera * cam )
//{
//    m_offscreenRenderLogicTst->SetRendererCamera( cam );
//}
//
//// *********************************
////
//void    RenderLogicTst::RenderFrame     ( Renderer * renderer, SceneNode * node )
//{
//    { renderer; }
//    { node; }
//    assert( false );
//
//    // FIXME: copy code from RenderLogicTst used by the application
//}
//
//// *********************************
////
//void    RenderLogicTst::RenderNode      ( Renderer * renderer, SceneNode * node )
//{
//    { renderer; }
//    { node; }
//    assert( false );
//
//    // FIXME: copy code from RenderLogicTst used by the application
//}
//
//// *********************************
////
//void    RenderLogicTst::RenderVanilla   ( Renderer * renderer, SceneNode * node )
//{
//    DrawNode( renderer, node );
//}
//
//// *********************************
////
//void    RenderLogicTst::RenderAlphaMask ( Renderer * renderer, SceneNode * node )
//{
//    { renderer; }
//    { node; }
//    assert( false );
//
//    // FIXME: copy code from RenderLogicTst used by the application
//}
//
//// *********************************
////
//void    RenderLogicTst::RenderNodeMask  ( Renderer * renderer, SceneNode * node )
//{
//    { renderer; }
//    { node; }
//    assert( false );
//
//    // FIXME: copy code from RenderLogicTst used by the application
//}
//
//// *********************************
////
//void    RenderLogicTst::RenderNodeMask1 ( Renderer * renderer, SceneNode * node )
//{
//    { renderer; }
//    { node; }
//    assert( false );
//
//    // FIXME: copy code from RenderLogicTst used by the application
//}
//
//// *********************************
////
//void    RenderLogicTst::DrawNode        ( Renderer * renderer, SceneNode * node )
//{
//	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
//    DrawNodeOnly( renderer, node );
//
//    DrawChildren( renderer, node );
//}
//
//// *********************************
////
//void    RenderLogicTst::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
//{
//    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
//}
//
//// *********************************
////
//void    RenderLogicTst::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx )
//{
//    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
//    {
//		HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
//        RenderNode( renderer, node->GetChild( i ) ); 
//    }
//}
//
//// *********************************
////
//void    RenderLogicTst::FrameRendered   ( Renderer * renderer )
//{
//    static int w = 0;
//    static int h = 0;
//
//    if( !DefaultConfig.ReadbackFlag() )
//    {
//        //Not needed as it does not make sense without readback delay
//        //m_offscreenRenderLogicTst->SwapDisplayRenderTargets();
//        return;
//    }
//
//    if( w != renderer->GetWidth() || h != renderer->GetHeight() )
//    {
//        w = renderer->GetWidth();
//        h = renderer->GetHeight();
//
//        printf( "Framebuffer resolution changed to %dx%d\n", w, h );
//    }
//
//    static double totalElapsed = 0.0;
//    static int nFrames = 1;
//    static int nPasses = 0;
//    static int nReadbackFrame = 0;
//
//    double readbackStart = GTimer.CurElapsed();
//    auto frame = m_offscreenRenderLogicTst->ReadDisplayTarget( renderer, nReadbackFrame );
//    nReadbackFrame = ( nReadbackFrame + 1 ) % m_offscreenRenderLogicTst->NumReadBuffersPerRT();
//    double readbackTime = GTimer.CurElapsed() - readbackStart;
//
//    m_offscreenRenderLogicTst->SwapDisplayRenderTargets();
//
//    totalElapsed += readbackTime;
//
//    //printf( "Time cur %.5f ms of total %.5f ms\n", 1000.f * readbackTime, 1000.f * totalElapsed );
//
//    if( nFrames % 50 == 0 )
//    {
//        //double avg = totalElapsed / (double) nFrames;
//
//        nPasses++;
//        totalElapsed = 0.0;
//
//        if ( nPasses % 3 == 0 )
//        {
//            nPasses = 0;
//
//            //printf( "Avg readback time from last %d frames took %.4f ms\n", nFrames, avg * 1000 );
//        }
//
//        nFrames = 0;
//    }
//
//    nFrames++;
//}
//
//// *********************************
////
//void    RenderLogicTst::PrintGLStats    (  bool detailed  )
//{
//    if ( detailed )
//    {
//        BVGL::PrintCompleteSummary( " ************************* DETAILED GL STATS ********************************** " );
//    }
//    else
//    {
//        BVGL::PrintShortSummary( " ************************* SHORT GL STATS ********************************** " );
//    }
//}
//
//} //bv
