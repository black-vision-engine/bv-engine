#include "RenderLogicImpl.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// ***************************
//
RenderLogicImpl::RenderLogicImpl     ( bool videoCardEnabled, RenderTargetStack * rtStack )
    : m_useVideoCard( videoCardEnabled )
    , m_rtStack( rtStack )
{

}

// ***************************
//
RenderLogicImpl::~RenderLogicImpl    ()
{
}

// ***************************
//
void    RenderLogicImpl::PreRenderFrame      ( Renderer * renderer )
{
    // Prepare frame for rendering
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
    renderer->PreDraw();
}

// ***************************
//
void    RenderLogicImpl::PostRenderFrame    ( Renderer * renderer )
{
    //m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
    //m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    //m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

} //bv
