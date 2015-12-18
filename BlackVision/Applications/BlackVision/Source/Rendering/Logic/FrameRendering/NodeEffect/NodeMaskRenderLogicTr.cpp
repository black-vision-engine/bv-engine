#include "NodeMaskRenderLogicTr.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/OffscreenRenderLogic.h"

#include "Rendering/Logic/FrameRendering/FrameRenderLogic.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"

#include "Rendering/Logic/FullScreen/Impl/BlitAlphaMaskFullscreenEffect.h"


namespace bv {

// *********************************
//
NodeMaskRenderLogicTr::NodeMaskRenderLogicTr        ()
    : m_blitAlphaMaskEffect( nullptr )
{
}

// *********************************
//
NodeMaskRenderLogicTr::~NodeMaskRenderLogicTr       ()
{
}

// *********************************
//
void    NodeMaskRenderLogicTr::RenderNode           ( SceneNode * node, RenderLogicContext * ctx )
{
    if( node->NumChildNodes() < 2 )
    {
        logic( ctx )->DrawNode( renderer( ctx ), node );
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
            logic->RenderChildren( renderer, node, 2 );
        }
        else
        {
            auto rtAllocator    = ctx->GetRenderTargetAllocator();

            logic->DrawNodeOnly( renderer, node );

            renderer->Disable( rtAllocator->Top() );

            auto foregroundRt   = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
            auto maskRt         = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );

            RenderItermediateData( ctx, foregroundRt, maskRt, node );

            rtAllocator->Free();
            rtAllocator->Free();

            BlitWithMask( ctx, foregroundRt, maskRt, alphaValue );

            logic->RenderChildren( renderer, node, 2 );
        }
    }
}

// *********************************
//
void                                NodeMaskRenderLogicTr::RenderItermediateData       ( RenderLogicContext * ctx, RenderTarget * foregroundRt, RenderTarget * maskRt, SceneNode * node )
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
void                                NodeMaskRenderLogicTr::RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node )
{
    auto renderer  = ctx->GetRenderer();

    renderer->Enable( rt );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    logic( ctx )->RenderNode( renderer, node ); 

    renderer->Disable( rt );
}

// *********************************
//
BlitAlphaMaskFullscreenEffect *     NodeMaskRenderLogicTr::AccessBlitAlphaMaskEffect   ( RenderTarget * rt, RenderTarget * maskRt )
{
    if ( !m_blitAlphaMaskEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );
        auto maskTex = maskRt->ColorTexture( 0 );

        m_blitAlphaMaskEffect = new BlitAlphaMaskFullscreenEffect( rtTex, maskTex );
    }

    return m_blitAlphaMaskEffect;    
}

// *********************************
//
void                                NodeMaskRenderLogicTr::BlitWithMask                ( RenderLogicContext * ctx, RenderTarget * foregroundRt, RenderTarget * maskRt, float alpha )
{
    auto renderer  = ctx->GetRenderer();

    auto blitter = AccessBlitAlphaMaskEffect( foregroundRt, maskRt );
    blitter->SetAlpha( alpha );

    renderer->Enable( allocator( ctx )->Top() );
    blitter->Render( renderer );
}

} //bv
