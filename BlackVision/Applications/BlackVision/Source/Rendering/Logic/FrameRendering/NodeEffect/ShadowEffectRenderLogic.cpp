#include "ShadowEffectRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Logic/FullScreen/Impl/ShadowFullscreenEffect.h"

#include "Rendering/Logic/FullScreen/Impl/BlurFullscreenEffect.h"

namespace bv {

// *********************************
//
ShadowEffectRenderLogic::ShadowEffectRenderLogic        ()
    : m_shadowEffect( nullptr )
    , m_blurEffect( nullptr )
{
}

// *********************************
//
ShadowEffectRenderLogic::~ShadowEffectRenderLogic       ()
{
}

// *********************************
//
void    ShadowEffectRenderLogic::RenderNode           ( SceneNode * node, RenderLogicContext * ctx )
{

    auto colorVal = node->GetNodeEffect()->GetValue( "color" );
    auto colorValue = QueryTypedValue< ValueVec4Ptr >( colorVal )->GetValue();

    auto shiftVal = node->GetNodeEffect()->GetValue( "shift" );
    auto shiftValue = QueryTypedValue< ValueVec2Ptr >( shiftVal )->GetValue();

    auto blurSizeVal = node->GetNodeEffect()->GetValue( "blurSize" );
    auto blurSizeValue = QueryTypedValue< ValueFloatPtr >( blurSizeVal )->GetValue();

    auto scaleX = glm::length( node->GetTransformable()->WorldTransforms()[ 0 ].Matrix() * glm::vec4( 1.f, 0.f, 0.f, 0.f ) );
    auto scaleY = glm::length( node->GetTransformable()->WorldTransforms()[ 0 ].Matrix() * glm::vec4( 0.f, 1.f, 0.f, 0.f ) );

    auto renderer       = ctx->GetRenderer();
    auto logic          = ctx->GetRenderLogic();
        
    if( colorValue.a < 0.01f )
    {
        logic->DrawNode( renderer, node, ctx );
    }
    else
    {
        auto rtAllocator    = ctx->GetRenderTargetAllocator();

        auto mainTarget = disableBoundRT( ctx );

        auto foregroundRt   = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

        RenderItermediateData( ctx, foregroundRt, node );

        auto vBluredRenderTarget = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

        {
            auto hBluredRenderTarget = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );    

            enable( ctx, hBluredRenderTarget );
            clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

            ApplyBlurEffect( renderer, foregroundRt, blurSizeValue * scaleX, false );

            rtAllocator->Free();

            disableBoundRT( ctx );

            enable( ctx, vBluredRenderTarget );
            clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

            ApplyBlurEffect( renderer, hBluredRenderTarget, blurSizeValue * scaleY, true );

            rtAllocator->Free();

            disableBoundRT( ctx );
        }

        rtAllocator->Free();

        enable( ctx, mainTarget );

        AddShadowEffect( renderer, foregroundRt->ColorTexture( 0 ), vBluredRenderTarget->ColorTexture( 0 ), colorValue, shiftValue );
    }
}

// *********************************
//
void                                ShadowEffectRenderLogic::RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * tr, SceneNode * node )
{
    RenderToRenderTarget( ctx, tr, node );
}

// *********************************
//
void                        ShadowEffectRenderLogic::RenderToRenderTarget   ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    enable( ctx, rt );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( renderer( ctx ), node, ctx ); 

    disableBoundRT( ctx );
}

// *********************************
//
ShadowFullscreenEffect *     ShadowEffectRenderLogic::AccessShadowEffect    ( Texture2DPtr tex, Texture2DPtr bluredTex, const glm::vec4 & color, const glm::vec2 & shift )
{
    if ( !m_shadowEffect )
    {
        m_shadowEffect = new ShadowFullscreenEffect( tex, bluredTex );
    }

    m_shadowEffect->SetColor( color );
    m_shadowEffect->SetShift( shift );

    return m_shadowEffect;    
}

// *********************************
//
void                                ShadowEffectRenderLogic::AddShadowEffect        ( Renderer * renderer, Texture2DPtr tex, Texture2DPtr bluredTex, const glm::vec4 & color, const glm::vec2 & shift )
{
    auto shadower = AccessShadowEffect( tex, bluredTex, color, shift );

    shadower->Render( renderer );
}

// *********************************
//
BlurFullscreenEffect *              ShadowEffectRenderLogic::AccessBlurEffect       ( RenderTarget * rt, float bs, bool vertical )
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
void                                ShadowEffectRenderLogic::ApplyBlurEffect        ( Renderer * renderer, RenderTarget * foregroundRt, float bs, bool vertical )
{
    auto blurer = AccessBlurEffect( foregroundRt, bs, vertical );

    blurer->Render( renderer );
}

} //bv
