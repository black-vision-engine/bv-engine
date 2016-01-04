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

        enable( ctx, mainTarget );

        ApplyBlurEffect( renderer, foregroundRt, blurSizeValue );
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
BlurFullscreenEffect *     BlurEffectRenderLogic::AccessBlurEffect      ( RenderTarget * rt, float bs )
{
    if ( !m_blurEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );

        m_blurEffect = new BlurFullscreenEffect( rtTex );
    }

    m_blurEffect->SetBlurSize( bs );  

    return m_blurEffect;    
}

// *********************************
//
void                        BlurEffectRenderLogic::ApplyBlurEffect      ( Renderer * renderer, RenderTarget * foregroundRt, float bs )
{
    auto shadower = AccessBlurEffect( foregroundRt, bs );

    shadower->Render( renderer );
}

} //bv
