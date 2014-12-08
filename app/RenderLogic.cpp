#include "RenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "System/HerarchicalProfiler.h"
#include "System/HRTimer.h"

#include "OffscreenRenderLogic.h"
#include "BVConfig.h"


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
    renderer->ClearBuffers();
    renderer->PreDraw();

    // AllocateNew()
    // EnableTop()
    //m_offscreenRenderLogic->EnableDisplayRenderTarget( renderer );

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->ClearBuffers();
    RenderNode( renderer, node );

    m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    // DisableTop()
    // DiscardCur()
    //m_offscreenRenderLogic->DisableDisplayRenderTarget( renderer );

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
        bool isOverriden = node->IsOverriden();
       
        //Render to auxiliary buffer
        if( isOverriden )
        {
            // AllocateNew()
            // EnableTop()

            //FIXME: generic approach requires that only current aux is disabled but some other aux target may still be enabled here
            //assert( m_offscreenRenderLogic->AuxRenderTargetEnabled() == false );
            //m_offscreenRenderLogic->EnableAuxRenderTarget( renderer );
            
            m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
            m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

            renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
            renderer->ClearBuffers();
        }

        DrawNode( renderer, node );

        //Blend top auxiliary buffer with the previous buffer
        if( isOverriden )
        {
            m_offscreenRenderLogic->DrawTopAuxRenderTarget( renderer, node->GetOverrideAlpha() );
            
            m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

            m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

            // DisableTop()
            // DiscardCur()

            // EnableCur()
            //FIXME: draw to the previous target and not to the explicitely stated display render target
            //m_offscreenRenderLogic->EnableDisplayRenderTarget( renderer );

            //FIXME: next two line force blending with specified alpha
            //m_offscreenRenderLogic->SetAuxAlphaModelValue( node->GetOverrideAlpha() );
            //m_offscreenRenderLogic->DrawAuxRenderTarget( renderer );

            //FIXME: use prev node to mask current node
            //m_offscreenRenderLogic->DrawAuxRenderTargetUsingPrevAlpha( renderer );
        }
    }
}

// *********************************
//
void    RenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node )
{
    HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor" );
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );

    for( int i = 0; i < node->NumTransformables(); ++i )
    {
        HPROFILER_SECTION( "RenderNode::renderer->Draw sibling" );
        renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
    }

    for ( int i = 0; i < node->NumChildrenNodes(); i++ )
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
