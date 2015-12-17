#include "AlphaMaskRenderLogicTr.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Logic/FullScreen/Impl/BlitFullscreenEffect.h"


namespace bv {

// *********************************
//
AlphaMaskRenderLogicTr::AlphaMaskRenderLogicTr      ()
    : m_blitEffect( nullptr )
{ 
}

// *********************************
//
AlphaMaskRenderLogicTr::~AlphaMaskRenderLogicTr     ()
{
}

// *********************************
//
void    AlphaMaskRenderLogicTr::RenderNode                  ( SceneNode * node, RenderLogicContext * ctx )
{
    auto alphaVal = node->GetNodeEffect()->GetValue( "alpha" );
    auto alphaValue = QueryTypedValue< ValueFloatPtr >( alphaVal )->GetValue();

    if( alphaValue > 0.99f )
    {
        ctx->GetRenderLogic()->DrawNode( ctx->GetRenderer(), node );
    }
    else if ( alphaValue > 0.01f )
    {
        auto renderer       = ctx->GetRenderer();
        auto rtAllocator    = ctx->GetRenderTargetAllocator();

        renderer->Disable( rtAllocator->Top() );

        auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        renderer->Enable( rt );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        ctx->GetRenderLogic()->DrawNode( renderer, node );

        renderer->Disable( rt );
        rtAllocator->Free();

        BlitWithAlpha( renderer, rtAllocator->Top(), rt, alphaValue );

        renderer->Enable( rtAllocator->Top() );
    }

    // Do not render this node if alpha is more or less equal to zero
}

// *********************************
//
BlitFullscreenEffect *  AlphaMaskRenderLogicTr::AccessBlitAlphaEffect   ( RenderTarget * rt, float alpha )
{
    if ( !m_blitEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );

        m_blitEffect = new BlitFullscreenEffect( rtTex, true );
    }

    m_blitEffect->SetAlpha( alpha );

    return m_blitEffect;    
}

// *********************************
//
void                    AlphaMaskRenderLogicTr::BlitWithAlpha           ( Renderer * renderer, RenderTarget * mainTarget, RenderTarget * alphaTarget, float alpha )
{
    assert( mainTarget != alphaTarget );

    auto blitter = AccessBlitAlphaEffect( alphaTarget, alpha );

    renderer->Enable( mainTarget );

    blitter->Render( renderer );

    renderer->Disable( mainTarget );
}

} //bv
