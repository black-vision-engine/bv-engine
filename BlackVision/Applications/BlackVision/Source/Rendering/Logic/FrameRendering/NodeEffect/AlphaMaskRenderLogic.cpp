#include "AlphaMaskRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Logic/FullScreen/Impl/BlitFullscreenEffect.h"


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
        ctx->GetRenderLogic()->DrawNode( ctx->GetRenderer(), node );
    }
    else if ( alphaValue > 0.01f )
    {
        Start( ctx );

        auto rt = RenderToRenderTarget( ctx, node );

        BlitWithAlpha( ctx, rt, alphaValue );

        Finalize( ctx );
    }

    // Do not render this node if alpha is more or less equal to zero
}

// *********************************
//
RenderTarget * AlphaMaskRenderLogic::RenderToRenderTarget         ( RenderLogicContext * ctx, SceneNode * node )
{
    auto renderer       = ctx->GetRenderer();
    auto rtAllocator    = ctx->GetRenderTargetAllocator();

    auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
    renderer->Enable( rt );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    logic( ctx )->DrawNode( renderer, node );

    renderer->Disable( rt );
    rtAllocator->Free();

    return rt;
}

// *********************************
//
void                    AlphaMaskRenderLogic::Start                   ( RenderLogicContext * ctx )
{
    renderer( ctx )->Disable( allocator( ctx )->Top() );    
}

// *********************************
//
void                    AlphaMaskRenderLogic::Finalize                ( RenderLogicContext * ctx )
{
    { ctx; }
    // renderer( ctx )->Enable( allocator( ctx )->Top() );
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
void                    AlphaMaskRenderLogic::BlitWithAlpha           ( RenderLogicContext * ctx, RenderTarget * alphaTarget, float alpha )
{
    assert( ctx->GetRenderTargetAllocator()->Top() != alphaTarget );

    auto mainTarget = ctx->GetRenderTargetAllocator()->Top();
    auto renderer = ctx->GetRenderer();

    auto blitter = AccessBlitAlphaEffect( alphaTarget, alpha );

    renderer->Enable( mainTarget );

    blitter->Render( renderer );

    //renderer->Disable( mainTarget );
}

} //bv
