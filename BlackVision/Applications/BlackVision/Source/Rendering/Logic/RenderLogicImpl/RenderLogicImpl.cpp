#include "RenderLogicImpl.h"

#include "Engine/Graphics/Renderers/Renderer.h"

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

    m_offscreenDisplay->EnableActiveRenderTarget( renderer );
}

// ***************************
//
void    RenderLogicImpl::RenderFrame        ( Renderer * renderer, SceneNode * sceneRoot )
{
    { renderer; sceneRoot; }
}

// ***************************
//
void    RenderLogicImpl::PostRenderFrame    ( Renderer * renderer )
{
    m_offscreenDisplay->DisableActiveRenderTarget( renderer );
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();

    //m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    //m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    //m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

} //bv
