#include "stdafx.h"

#include "RenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"
#include "Engine/Graphics/Effects/Utils/OffscreenDisplay.h"
#include "Engine/Audio/AudioRenderer.h"

#include "Engine/Graphics/Effects/Logic/VideoOutputRendering/VideoOutputRenderLogic.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectFactory.h"

#include "Engine/Graphics/Shaders/RenderablePass.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"
#include "Engine/Graphics/Effects/BoundingBoxEffect.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
//#include "FrameStatsService.h"

#include "UseLoggerLibBlackVision.h"

#include "LibImage.h"


#include "Memory/MemoryLeaks.h"


namespace bv {

// *********************************
//                             DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.ClearColor(), DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput(), DefaultConfig.RenderToSharedMemory(), DefaultConfig.SharedMemoryScaleFactor());
RenderLogic::RenderLogic     ( unsigned int width, unsigned int height, const glm::vec4 & clearColor, bool useReadback, bool useVideoCardOutput, bool enableSharedMemory, int scaleFactor )
    : m_rtStackAllocator( width, height, TextureFormat::F_A8R8G8B8 )
    , m_blitEffect( nullptr )
    , m_videoOutputRenderLogic( nullptr )
    , m_ctx( nullptr )
    , m_clearColor( clearColor )
    , m_enableSharedMemory( enableSharedMemory )
    , m_sharedMemoryVideoBuffer( nullptr )
	, m_sharedMemoryScaleFactor(scaleFactor)
{
    auto videoCardEnabled   = useReadback;
    auto previewAsVideoCard = useVideoCardOutput;

    unsigned int numFrameRenderTargets = videoCardEnabled || previewAsVideoCard ? 2 : 1;

    m_blitEffect                = CreateFullscreenEffectInstance( FullscreenEffectType::FET_SIMPLE_BLIT );
    m_offscreenDisplay          = new OffscreenDisplay( &m_rtStackAllocator, numFrameRenderTargets, videoCardEnabled || previewAsVideoCard );
    m_videoOutputRenderLogic    = new VideoOutputRenderLogic( height ); // FIXME: interlace odd/even setup

    m_displayVideoCardPreview   = previewAsVideoCard;
    m_useVideoCardOutput        = videoCardEnabled;

    auto effect = new BoundingBoxEffect(); // FIXME: memory leak
    m_boundingBoxEffect         = effect->GetPass( 0 );

    if( m_enableSharedMemory )
    {
        // FIXME: nrl - deprecated
        m_sharedMemoryVideoBuffer = nullptr; //new SharedMemoryVideoBuffer( width, height, TextureFormat::F_A8R8G8B8, m_sharedMemoryScaleFactor);
    }
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    delete m_offscreenDisplay;
    delete m_videoOutputRenderLogic;
    delete m_blitEffect;
    delete m_ctx;
    
    if( m_sharedMemoryVideoBuffer )
    {
        delete m_sharedMemoryVideoBuffer;
    }
}

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes )
{
    renderer->PreDraw();

    RenderFrameImpl( renderer, audioRenderer, scenes );
    
    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    RenderLogic::RenderFrameImpl ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes )
{
    auto rt = m_offscreenDisplay->GetCurrentFrameRenderTarget();

    RenderRootNode( renderer, audioRenderer, scenes, rt );

    FrameRendered( renderer, audioRenderer );

    UpdateOffscreenState();
}

// *********************************
//
//  if not DisplayAsVideoOutput:
//      BlitToWindow()
//  else:
//      GPURenderPreVideo()
//      BlitToWindow()
//
//      if PushToVideoCard:
//          Readback()
//          Push()
//
void    RenderLogic::FrameRendered   ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
    auto prevRt = m_offscreenDisplay->GetCurrentFrameRenderTarget();
    auto ctx = GetContext( renderer, audioRenderer );

    if( m_displayVideoCardPreview )
    {
        auto videoRt    = m_offscreenDisplay->GetVideoRenderTarget          ();
        auto curFrameRt = m_offscreenDisplay->GetCurrentFrameRenderTarget   ();
        auto prvFrameRt = m_offscreenDisplay->GetPreviousFrameRenderTarget  ();

        m_videoOutputRenderLogic->Render( videoRt, curFrameRt, prvFrameRt, ctx );

        prevRt = videoRt;
    }

    BlitToPreview( prevRt, ctx );

    if( m_useVideoCardOutput )
    {
        // FIXME: nrl - audio should be handled based on this implementation
        OnVideoFrameRendered( ctx );
    }
}

// *********************************
//
void    RenderLogic::RenderRootNode  ( Renderer * renderer, audio::AudioRenderer * audioRenderer, const SceneVec & scenes, RenderTarget * rt )
{
    { renderer; audioRenderer; scenes; rt; }
    // FIXME: nrl - implement in NNodeRenderLogic
    assert( false );
    ////FIXME: assumes only one renderer instance per application
    //auto ctx = GetContext( renderer, audioRenderer );

    //assert( renderer == ctx->GetRenderer() );

    //// FIXME: verify that all rendering paths work as expected
    //enable( ctx, rt );
    //    
    //clearBoundRT( ctx, m_clearColor );

    //for( auto & scene : scenes )
    //{
    //    renderer->Performance().AverageScenePerformanceData( scene );

    //    renderer->SetCamera( scene->GetCamera() );

    //    renderer->EnableScene( scene );
    //    auto renderQueue = ctx->GetRenderQueueAllocator()->Allocate();

    //    renderQueue->QueueNodeSubtree( scene->GetRoot(), ctx );
    //    renderQueue->Render( ctx );
    //    
    //    ctx->GetRenderQueueAllocator()->Free();
    //    
    //    RenderGridLines( scene, ctx );          // FIXME: Use some generic solution when other editor helper object apear in engine.
    // 
    //    Play( audioRenderer, scene->GetRoot() );
    //}

    //disableBoundRT( ctx );
}


namespace {

bool IsSelected( SceneNode * node ) 
{ 
    return node->IsSelected();
}

glm::vec4 BoundingBoxColor( SceneNode * node )
{
    return node->GetBoundingBoxColor();
}

} // anonymous

// *********************************
//
void    RenderLogic::RenderNode      ( SceneNode * node, RenderLogicContext * ctx )
{
    // FIXME: nrl - implement in NNodeRenderLogic
    {node; ctx;}
    //if ( node->IsVisible() )
    //{
    //    auto effect = node->GetNodeEffect();

    //    if( !effect || effect->GetType() ==  NodeEffectType::NET_DEFAULT )
    //    {
    //        // Default render logic
    //        DrawNode( node, ctx );
    //    }
    //    else
    //    {
    //        effect->Render( node, ctx );
    //    }

    //    if( IsSelected( node ) && Cast< RenderableEntity* >( node->GetTransformable() )->GetRenderableEffect() != nullptr )
    //        RenderBoundingBox( node, ctx, BoundingBoxColor( node ) );
    //}
}

// *********************************
//
void    RenderLogic::DrawNode        ( SceneNode * node, RenderLogicContext * ctx )
{
    HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer( ctx ), node );

    RenderChildren( node, ctx );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly    ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::RenderChildren  ( SceneNode * node, RenderLogicContext * ctx, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( node->GetChild( i ), ctx ); 
    }
}

// *********************************
//
void    RenderLogic::Play           ( audio::AudioRenderer * renderer, SceneNode * node )
{
    // FIXME: and what about node effects???
    if ( node->IsVisible() )
    {
        auto audio = node->GetAudio();
        if( audio )
        {
            renderer->Proccess( audio );
        }

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
        {
            Play( renderer, node->GetChild( i ) );
        }
    }
}

// ***********************
//
void    RenderLogic::RenderBoundingBox  ( SceneNode * node, RenderLogicContext * ctx, glm::vec4 color )
{
    auto obj = Cast< RenderableEntityWithBoundingBox * >( node->GetTransformable() );
    assert( obj );

    auto bb = obj->GetBoundingBox();
    if( bb )
    {
        auto renderer = ctx->GetRenderer();

        auto param = Cast< ShaderParamVec4 * >( m_boundingBoxEffect->GetPixelShader()->GetParameters()->AccessParam( "color" ) );
        param->SetValue( color );

        renderer->Enable( m_boundingBoxEffect, bb );
        renderer->DrawRenderable( bb );

        auto wc = obj->GetCenterOfMass();
        renderer->Enable( m_boundingBoxEffect, wc );
        renderer->DrawRenderable( wc );
    }
}

// ***********************
//
void    RenderLogic::RenderGridLines    ( Scene * scene, RenderLogicContext * ctx )
{
    if( scene->GetGridLinesVisibility() )
    {
        auto renderable = scene->GetGridLines();
        auto renderer = ctx->GetRenderer();

        renderer->Draw( renderable );
    }
}


// *********************************
//
RenderLogicContext *    RenderLogic::GetContext         ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
    if( !m_ctx )
    {
        m_ctx = new RenderLogicContext( renderer, &m_rtStackAllocator, &m_renderQueueAllocator, this, audioRenderer );
    }

    return m_ctx;
}

// *********************************
//
void                    RenderLogic::BlitToPreview          ( RenderTarget * rt, RenderLogicContext * ctx )
{
    if( m_blitEffect->GetRenderTarget( 0 ) != rt )
    {
        m_blitEffect->SetRenderTarget( 0, rt );
    }

    // Clear 'preview' render target.
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

    m_blitEffect->Render( rt, ctx );
}

// *********************************
//
void                    RenderLogic::UpdateOffscreenState   ()
{
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();
    m_offscreenDisplay->UpdateVideoRenderTargetIdx();
}

// *********************************
//
void                    RenderLogic::OnVideoFrameRendered   ( RenderLogicContext * ctx )
{
    auto rt = m_offscreenDisplay->GetVideoRenderTarget();
	{
		HPROFILER_SECTION("Shared Memory Copy", PROFILER_THREAD1);
		if (m_enableSharedMemory)
		{
			if(m_videoOutputRenderLogic->GetLastVideoFrame()!=nullptr)
				m_sharedMemoryVideoBuffer->PushFrame(m_videoOutputRenderLogic->GetLastVideoFrame());
		}
	}

	{
		HPROFILER_SECTION("Wait for Sync", PROFILER_THREAD1);

		m_videoOutputRenderLogic->VideoFrameRendered(rt, ctx);
	}
	

   

}

// *********************************
//
VideoOutputRenderLogic *    RenderLogic::GedVideoOutputRenderLogic  ()
{
    return m_videoOutputRenderLogic;
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
