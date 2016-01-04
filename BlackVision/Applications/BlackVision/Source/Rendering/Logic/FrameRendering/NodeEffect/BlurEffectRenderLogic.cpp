#include "BlurEffectRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Logic/FullScreen/Impl/BlurFullscreenEffect.h"


namespace bv {

// *********************************
//
BlurEffectRenderLogic::BlurEffectRenderLogic        ()
    : m_blurEffect( nullptr )
{
}

// *********************************
//
BlurEffectRenderLogic::~BlurEffectRenderLogic       ()
{
}

// *********************************
//
void                        BlurEffectRenderLogic::RenderNode           ( SceneNode * node, RenderLogicContext * ctx )
{
    auto blurSizeVal = node->GetNodeEffect()->GetValue( "blurSize" );
    auto blurSizeValue = QueryTypedValue< ValueFloatPtr >( blurSizeVal )->GetValue();

    auto renderer       = ctx->GetRenderer();
    auto logic          = ctx->GetRenderLogic();
        
    if( blurSizeValue < 1.f )
    {
        logic->DrawNode( renderer, node, ctx );
    }
    else
    {
        auto rtAllocator    = ctx->GetRenderTargetAllocator();

        auto mainTarget = disableBoundRT( ctx );

        auto foregroundRt   = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

        RenderItermediateData( ctx, foregroundRt, node );

        rtAllocator->Free();

        auto hBluredRenderTarget = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

        enable( ctx, hBluredRenderTarget );

        ApplyBlurEffect( renderer, foregroundRt, blurSizeValue, true );

        rtAllocator->Free();

        disableBoundRT( ctx );

        enable( ctx, mainTarget );

        ApplyBlurEffect( renderer, hBluredRenderTarget, blurSizeValue, false );
    }
}

// *********************************
//
void                        BlurEffectRenderLogic::RenderItermediateData( RenderLogicContext * ctx, RenderTarget * tr, SceneNode * node )
{
    RenderToRenderTarget( ctx, tr, node );
}

// *********************************
//
void                        BlurEffectRenderLogic::RenderToRenderTarget ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    enable( ctx, rt );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( renderer( ctx ), node, ctx ); 

    disableBoundRT( ctx );
}

// *********************************
//
BlurFullscreenEffect *      BlurEffectRenderLogic::AccessBlurEffect      ( RenderTarget * rt, float bs, bool vertical )
{
    auto rtTex = rt->ColorTexture( 0 );

    if ( !m_blurEffect )
    {
        m_blurEffect = new BlurFullscreenEffect( rtTex );
    }

    m_blurEffect->SetTexture( rtTex );

    m_blurEffect->SetBlurSize( bs );
    m_blurEffect->SetVertical( vertical );

    return m_blurEffect;    
}

// *********************************
//
void                        BlurEffectRenderLogic::ApplyBlurEffect      ( Renderer * renderer, RenderTarget * foregroundRt, float bs, bool vertical )
{
    auto blurer = AccessBlurEffect( foregroundRt, bs, vertical );

    blurer->Render( renderer );
}

} //bv
