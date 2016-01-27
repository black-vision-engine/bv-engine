#include "RenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"
#include "Engine/Graphics/Effects/Utils/OffscreenDisplay.h"

#include "Engine/Graphics/Effects/FullScreen/Old/Impl/BlitFullscreenEffect.h"
#include "Engine/Graphics/Effects/Logic/VideoOutputRendering/VideoOutputRenderLogic.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

//FIXME: remove
#include "LibImage.h"
#include "Engine/Graphics/Resources/Texture2DImpl.h"


namespace bv {

// *********************************
//DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput()
RenderLogic::RenderLogic     ( unsigned int width, unsigned int height, bool useReadback, bool useVideoCardOutput )
    : m_rtStackAllocator( width, height, TextureFormat::F_A8R8G8B8 )
    , m_blitEffect( nullptr )
    , m_videoOutputRenderLogic( nullptr )
{
    auto videoCardEnabled   = useReadback;
    auto previewAsVideoCard = useVideoCardOutput;

    unsigned int numFrameRenderTargets = videoCardEnabled || previewAsVideoCard ? 2 : 1;

    m_offscreenDisplay          = new OffscreenDisplay( &m_rtStackAllocator, numFrameRenderTargets, videoCardEnabled || previewAsVideoCard );
    m_videoOutputRenderLogic    = new VideoOutputRenderLogic( height ); // FIXME: interlace odd/even setup

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
        OnVideoFrameRendered( renderer );
    }
}

// *********************************
//
void    RenderLogic::RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt )
{
    //FIXME: assumes only one renderer instance per application
    static RenderLogicContext ctx_( renderer, &m_rtStackAllocator, this );
    static RenderLogicContext * ctx = &ctx_;

    assert( renderer == ctx_.GetRenderer() );

    // FIXME: verify that all rendering paths work as expected
	if( sceneRoot )
    {
        enable( ctx, rt );

        clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

        RenderNode( sceneRoot, ctx );

        disableBoundRT( ctx );
    }
}

// *********************************
//
void    RenderLogic::RenderNode      ( SceneNode * node, RenderLogicContext * ctx )
{
    if ( node->IsVisible() )
    {
        auto effect = node->GetNodeEffect();

        if( !effect || effect->GetType() ==  NodeEffectType::NET_DEFAULT )
        {
            // Default render logic
            DrawNode( node, ctx );
        }
        else
        {
            effect->Render( node, ctx );
        }

        //FIXME: remove these files
        //auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogic( node );
        //effectRenderLogic->RenderNode( node, ctx );
    }
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
void                    RenderLogic::UpdateOffscreenState   ()
{
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();
    m_offscreenDisplay->UpdateVideoRenderTargetIdx();
}

// *********************************
//
void                    RenderLogic::OnVideoFrameRendered   ( Renderer * renderer )
{
    auto rt = m_offscreenDisplay->GetVideoRenderTarget();

    m_videoOutputRenderLogic->VideoFrameRendered( renderer, rt );
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
