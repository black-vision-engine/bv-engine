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
    auto normalizeVal = node->GetNodeEffect()->GetValue( "normalize" );
    auto blurSizeValue = QueryTypedValue< ValueFloatPtr >( blurSizeVal )->GetValue();
    auto normalizeFlagValue = QueryTypedValue< ValueIntPtr >( normalizeVal )->GetValue() > 0 ? true : false;

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
        auto hBluredRenderTarget = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

        RenderItermediateData( ctx, foregroundRt, node );

        enable( ctx, hBluredRenderTarget );
        clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

        ApplyBlurEffect( renderer, foregroundRt, blurSizeValue, false, normalizeFlagValue );

        rtAllocator->Free();
        rtAllocator->Free();

        disableBoundRT( ctx );

        enable( ctx, mainTarget );

        ApplyBlurEffect( renderer, hBluredRenderTarget, blurSizeValue, true, normalizeFlagValue );
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
BlurFullscreenEffect *      BlurEffectRenderLogic::AccessBlurEffect      ( RenderTarget * rt, float bs, bool vertical, bool normalize )
{
    auto rtTex = rt->ColorTexture( 0 );

    if ( !m_blurEffect )
    {
        m_blurEffect = new BlurFullscreenEffect( rtTex );
    }

    m_blurEffect->SetTexture( rtTex );

    m_blurEffect->SetBlurSize( bs );
    m_blurEffect->SetVertical( vertical );
    m_blurEffect->SetNormalize( normalize );

    return m_blurEffect;    
}

// *********************************
//
void                        BlurEffectRenderLogic::ApplyBlurEffect      ( Renderer * renderer, RenderTarget * foregroundRt, float bs, bool vertical, bool normalize )
{
    auto blurer = AccessBlurEffect( foregroundRt, bs, vertical, normalize );

    blurer->Render( renderer );
}

} //bv
