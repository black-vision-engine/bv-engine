#include "AlphaMaskRenderLogicTr.h"

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"



namespace bv {

// *********************************
//
AlphaMaskRenderLogicTr::AlphaMaskRenderLogicTr      ()
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

        auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        renderer->Enable( rt );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        ctx->GetRenderLogic()->DrawNode( renderer, node );

        auto alphaVal = node->GetNodeEffect()->GetValue( "alpha" );
        //GetOffscreenRenderLogic()->DrawTopAuxRenderTarget( renderer, alphaVal.get() );
        renderer->Disable( rt );
        
        renderer->Enable( rtAllocator->Top() );

        //GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        //GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
    }

    // Do not render this node if alpha is more or less equal to zero
}

} //bv
