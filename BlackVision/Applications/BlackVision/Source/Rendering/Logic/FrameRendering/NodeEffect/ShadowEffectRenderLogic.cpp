#include "ShadowEffectRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Logic/FullScreen/Impl/ShadowFullscreenEffect.h"


namespace bv {

// *********************************
//
ShadowEffectRenderLogic::ShadowEffectRenderLogic        ()
    : m_shadowEffect( nullptr )
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

    auto renderer       = ctx->GetRenderer();
    auto logic          = ctx->GetRenderLogic();
        
    if( colorValue.a < 0.01f )
    {
        logic->RenderNode( renderer, node, ctx );
    }
    else
    {
        auto rtAllocator    = ctx->GetRenderTargetAllocator();

        logic->DrawNodeOnly( renderer, node );

        auto mainTarget = disableBoundRT( ctx );

        auto foregroundRt   = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

        RenderItermediateData( ctx, foregroundRt, node );

        rtAllocator->Free();

        enable( ctx, mainTarget );

        AddShadowEffect( renderer, foregroundRt, colorValue, shiftValue, blurSizeValue );

        logic->RenderNode( renderer, node, ctx );
    }
}

// *********************************
//
void                                ShadowEffectRenderLogic::RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, SceneNode * node )
{
    RenderToRenderTarget( ctx, foregroundRt, node );
}

// *********************************
//
void                        ShadowEffectRenderLogic::RenderToRenderTarget   ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    enable( ctx, rt );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->RenderNode( renderer( ctx ), node, ctx ); 

    disableBoundRT( ctx );
}

// *********************************
//
ShadowFullscreenEffect *     ShadowEffectRenderLogic::AccessShadowEffect    ( RenderTarget * rt, const glm::vec4 & color, const glm::vec2 & shift, float bs )
{
    if ( !m_shadowEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );

        m_shadowEffect = new ShadowFullscreenEffect( rtTex );
    }

    m_shadowEffect->SetColor( color );
    m_shadowEffect->SetShift( shift );
    m_shadowEffect->SetBlurSize( bs );

    return m_shadowEffect;    
}

// *********************************
//
void                                ShadowEffectRenderLogic::AddShadowEffect        ( Renderer * renderer, RenderTarget * foregroundRt, const glm::vec4 & color, const glm::vec2 & shift, float bs )
{
    auto shadower = AccessShadowEffect( foregroundRt, color, shift, bs );

    shadower->Render( renderer );
}

} //bv
