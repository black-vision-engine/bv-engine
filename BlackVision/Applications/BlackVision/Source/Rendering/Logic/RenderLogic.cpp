#include "RenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Utils/RenderLogicContext.h"
#include "Rendering/Utils/OffscreenDisplay.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"
#include "Rendering/Logic/FullScreen/Impl/BlitFullscreenEffect.h"
#include "Rendering/Logic/VideoOutputRendering/VideoOutputRenderLogic.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

#include "BVConfig.h"


namespace bv {

// *********************************
//
RenderLogic::RenderLogic     ()
    : m_rtStackAllocator( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), TextureFormat::F_A8R8G8B8 )
    , m_blitEffect( nullptr )
    , m_videoOutputRenderLogic( nullptr )
{
    auto videoCardEnabled   = DefaultConfig.ReadbackFlag();
    auto previewAsVideoCard = DefaultConfig.DisplayVideoCardOutput(); // || true;

    unsigned int numFrameRenderTargets = videoCardEnabled || previewAsVideoCard ? 2 : 1;

    m_offscreenDisplay          = new OffscreenDisplay( &m_rtStackAllocator, numFrameRenderTargets, videoCardEnabled || previewAsVideoCard );
    m_videoOutputRenderLogic    = new VideoOutputRenderLogic( DefaultConfig.DefaultHeight() ); // FIXME: interlace odd/even setup

    m_displayVideoCardPreview   = previewAsVideoCard;
    m_useVideoCardOutput        = videoCardEnabled;
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    delete m_offscreenDisplay;
    delete m_blitEffect;
}

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    renderer->PreDraw();

    RenderFrameImpl( renderer, sceneRoot );
    
    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    RenderLogic::RenderFrameImpl ( Renderer * renderer, SceneNode * sceneRoot )
{
    auto rt = m_offscreenDisplay->GetCurrentFrameRenderTarget();

    RenderRootNode( renderer, sceneRoot, rt );

    FrameRendered( renderer );

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
void    RenderLogic::FrameRendered   ( Renderer * renderer )
{
    auto prevRt = m_offscreenDisplay->GetCurrentFrameRenderTarget();

    if( m_displayVideoCardPreview )
    {
        auto videoRt    = m_offscreenDisplay->GetVideoRenderTarget          ();
        auto curFrameRt = m_offscreenDisplay->GetCurrentFrameRenderTarget   ();
        auto prvFrameRt = m_offscreenDisplay->GetPreviousFrameRenderTarget  ();

        m_videoOutputRenderLogic->Render( renderer, videoRt, curFrameRt, prvFrameRt );

        prevRt = videoRt;
    }
     
    BlitToPreview( renderer, prevRt );

    if( m_useVideoCardOutput )
    {
        //FIXME: VIDEO CART CODE (PUSH FRAME) to be placed here
    }
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
        if( node->GetNodeEffect()->GetType() == NodeEffect::Type::T_DEFAULT )
        {
            // Default render logic
            DrawNode( renderer, node );
        }
        else
        {
            auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogic( node );
               
            effectRenderLogic->RenderNode( node, &ctx );
        }
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
    // FIXME: Blit current render target - suxx a bit - there should be a separate initialization step
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