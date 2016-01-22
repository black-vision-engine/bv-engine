#include "NodeMaskRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Graphics/Effects/FullScreen/Impl/BlitAlphaMaskFullscreenEffect.h"


namespace bv {

// *********************************
//
NodeMaskRenderLogic::NodeMaskRenderLogic        ()
    : m_blitAlphaMaskEffect( nullptr )
{
}

// *********************************
//
NodeMaskRenderLogic::~NodeMaskRenderLogic       ()
{
}

// *********************************
//
void    NodeMaskRenderLogic::RenderNode           ( SceneNode * node, RenderLogicContext * ctx )
{
    if( node->NumChildNodes() < 2 )
    {
        logic( ctx )->DrawNode( node, ctx );
    }
    else 
    {
        auto alphaVal = node->GetNodeEffect()->GetValue( "alpha" );
        auto alphaValue = QueryTypedValue< ValueFloatPtr >( alphaVal )->GetValue();
       
        auto renderer       = ctx->GetRenderer();
        auto logic          = ctx->GetRenderLogic();
        
        if( alphaValue < 0.01f )
        {
            logic->DrawNodeOnly( renderer, node );
            logic->RenderChildren( node, ctx, 2 );
        }
        else
        {
            auto rtAllocator    = ctx->GetRenderTargetAllocator();

            logic->DrawNodeOnly( renderer, node );

            auto mainTarget = disableBoundRT( ctx );

            auto foregroundRt   = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
            auto maskRt         = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

            RenderItermediateData( ctx, foregroundRt, maskRt, node );

            rtAllocator->Free();
            rtAllocator->Free();

            enable( ctx, mainTarget );

            BlitWithMask( renderer, foregroundRt, maskRt, alphaValue );

            logic->RenderChildren( node, ctx, 2 );
        }
    }
}

// *********************************
//
void                                NodeMaskRenderLogic::RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, RenderTarget * maskRt, SceneNode * node )
{
    auto effect = node->GetNodeEffect();

    auto maskIdxVal = std::static_pointer_cast< ValueInt >( effect->GetValue( "bgIdx" ) );
    auto fgIdxVal   = std::static_pointer_cast< ValueInt >( effect->GetValue( "fgIdx" ) );

    auto maskIdx = maskIdxVal->GetValue();
    auto fgIdx   = fgIdxVal->GetValue();

    assert( maskIdx == 0 || maskIdx == 1 );
    assert( fgIdx == 0 || fgIdx == 1 );
    assert( maskIdx != fgIdx );

    RenderToRenderTarget( ctx, foregroundRt, node->GetChild( fgIdx ) );
    RenderToRenderTarget( ctx, maskRt, node->GetChild( maskIdx ) );
}

// *********************************
//
void                                NodeMaskRenderLogic::RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    enable( ctx, rt );
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    logic( ctx )->RenderNode( node, ctx ); 

    disableBoundRT( ctx );
}

// *********************************
//
BlitAlphaMaskFullscreenEffect *     NodeMaskRenderLogic::AccessBlitAlphaMaskEffect   ( RenderTarget * rt, RenderTarget * maskRt, float alpha )
{
    if ( !m_blitAlphaMaskEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );
        auto maskTex = maskRt->ColorTexture( 0 );

        m_blitAlphaMaskEffect = new BlitAlphaMaskFullscreenEffect( rtTex, maskTex );
    }

    m_blitAlphaMaskEffect->SetAlpha( alpha );

    return m_blitAlphaMaskEffect;    
}

// *********************************
//
void                                NodeMaskRenderLogic::BlitWithMask                ( Renderer * renderer, RenderTarget * foregroundRt, RenderTarget * maskRt, float alpha )
{
    auto blitter = AccessBlitAlphaMaskEffect( foregroundRt, maskRt, alpha );

    blitter->Render( renderer );
}

} //bv