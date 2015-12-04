#include "RenderLogicImpl.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"

#include "Rendering/Utils/OffscreenDisplay.h"


namespace bv {

// ***************************
//
RenderLogicImpl::RenderLogicImpl     ( RenderTargetStackAllocator * rtStackAllocator, bool useTwoDisplayRenderTargets )
    : m_rtStackAllocator( rtStackAllocator )
{
    m_offscreenDisplay = new OffscreenDisplay( m_rtStackAllocator, useTwoDisplayRenderTargets );
}

// ***************************
//
RenderLogicImpl::~RenderLogicImpl    ()
{
    delete m_offscreenDisplay;
}

// ***************************
//
void    RenderLogicImpl::PreRenderFrame      ( Renderer * renderer )
{
    // Prepare frame for rendering
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
    renderer->PreDraw();
    
    renderer->Enable( m_offscreenDisplay->GetActiveRenderTarget() );
}

// ***************************
//
void    RenderLogicImpl::RenderFrame        ( Renderer * renderer, SceneNode * sceneRoot )
{
    PreRenderFrame( renderer );

	if( sceneRoot )
		RenderNode( renderer, sceneRoot );

    PostRenderFrame( renderer );
}

// ***************************
//
void    RenderLogicImpl::RenderNode          ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogic( node );
        
        effectRenderLogic->RenderNode( renderer, node );
    }
}

// ***************************
//
void    RenderLogicImpl::PostRenderFrame    ( Renderer * renderer )
{
    renderer->Disable( m_offscreenDisplay->GetActiveRenderTarget() );
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();

    //m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    //m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    //m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

} //bv
