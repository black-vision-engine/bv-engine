#include "AlphaMaskRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Graphics/Effects/FullScreen/Impl/BlitFullscreenEffect.h"


namespace bv {

// *********************************
//
AlphaMaskRenderLogic::AlphaMaskRenderLogic      ()
    : m_blitEffect( nullptr )
{ 
}

// *********************************
//
AlphaMaskRenderLogic::~AlphaMaskRenderLogic     ()
{
}

// *********************************
//
void    AlphaMaskRenderLogic::RenderNode                  ( SceneNode * node, RenderLogicContext * ctx )
{
    auto alphaVal = node->GetNodeEffect()->GetValue( "alpha" );
    auto alphaValue = QueryTypedValue< ValueFloatPtr >( alphaVal )->GetValue();

    if( alphaValue > 0.99f )
    {
        ctx->GetRenderLogic()->DrawNode( node, ctx );
    }
    else if ( alphaValue > 0.01f )
    {
        auto renderer = ctx->GetRenderer();
        auto mainTarget = disableBoundRT( ctx );

        auto intermediateTarget = RenderToRenderTarget( ctx, node );

        enable( ctx, mainTarget );

        BlitWithAlpha( renderer, intermediateTarget, alphaValue );
    }

    // Do not render this node if alpha is more or less equal to zero
}

// *********************************
//
RenderTarget * AlphaMaskRenderLogic::RenderToRenderTarget         ( RenderLogicContext * ctx, SceneNode * node )
{
    auto rtAllocator    = ctx->GetRenderTargetAllocator();

    auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
    
    enable( ctx, rt );

    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->DrawNode( node, ctx );

    disableBoundRT( ctx );
    rtAllocator->Free();

    return rt;
}

// *********************************
//
BlitFullscreenEffect *  AlphaMaskRenderLogic::AccessBlitAlphaEffect   ( RenderTarget * rt, float alpha )
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
void                    AlphaMaskRenderLogic::BlitWithAlpha           ( Renderer * renderer, RenderTarget * alphaTarget, float alpha )
{
    auto blitter = AccessBlitAlphaEffect( alphaTarget, alpha );
    blitter->Render( renderer );

    //renderer->Disable( mainTarget );
}

} //bv
