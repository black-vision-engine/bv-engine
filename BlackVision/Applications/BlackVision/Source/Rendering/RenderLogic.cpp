#include "RenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "System/HerarchicalProfiler.h"
#include "System/HRTimer.h"

#include "OffscreenRenderLogic.h"
#include "BVConfig.h"


#define USE_HACK_FRIEND_NODE_MASK_IMPL

namespace bv {

extern HighResolutionTimer GTimer;

// *********************************
//
RenderLogic::RenderLogic     ()
{
    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
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
void    RenderLogic::RenderFrame     ( Renderer * renderer, SceneNode * node )
{
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
    renderer->PreDraw();

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->ClearBuffers();
    RenderNode( renderer, node );

    m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    m_offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    RenderLogic::RenderNode      ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        if( node->IsOverridenAM() )
        {
            RenderAlphaMask( renderer, node );
        }
        else if( node->IsOverridenNM() )
        {
#ifndef USE_HACK_FRIEND_NODE_MASK_IMPL
            RenderNodeMask( renderer, node );
 #else
            RenderNodeMask1( renderer, node );
#endif
        }
        else
        {
            RenderVanilla( renderer, node );
        }
    }
}

// *********************************
//
void    RenderLogic::RenderVanilla   ( Renderer * renderer, SceneNode * node )
{
    DrawNode( renderer, node );
}

// *********************************
//
void    RenderLogic::RenderAlphaMask ( Renderer * renderer, SceneNode * node )
{
    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    DrawNode( renderer, node );

    m_offscreenRenderLogic->DrawTopAuxRenderTarget( renderer, node->GetOverrideAlphaVal() );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
}

// *********************************
//
void    RenderLogic::RenderNodeMask  ( Renderer * renderer, SceneNode * node )
{
    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
        
    DrawChildren( renderer, node );

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    DrawNodeOnly( renderer, node );

    m_offscreenRenderLogic->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
    
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
}

// *********************************
//
void    RenderLogic::RenderNodeMask1 ( Renderer * renderer, SceneNode * node )
{
    if( node->NumChildrenNodes() < 2 )
    {
        RenderVanilla( renderer, node );
    }
    else
    {
        DrawNodeOnly( renderer, node );

        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();
        
        // MASK
        RenderNode( renderer, node->GetChild( 1 ) ); 

        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        // FOREGROUND
        RenderNode( renderer, node->GetChild( 0 ) ); 

        m_offscreenRenderLogic->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
    
        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

        DrawChildren( renderer, node, 2 );
    }
}

// *********************************
//
void    RenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node )
{
    HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor" );
    DrawNodeOnly( renderer, node );

    DrawChildren( renderer, node );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );

    for( int i = 0; i < node->NumTransformables(); ++i )
    {
        HPROFILER_SECTION( "RenderNode::renderer->Draw sibling" );
        renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
    }
}

// *********************************
//
void    RenderLogic::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx )
{
    for ( int i = firstChildIdx; i < node->NumChildrenNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode" );
        RenderNode( renderer, node->GetChild( i ) ); 
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
        double avg = totalElapsed / (double) nFrames;

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

} //bv