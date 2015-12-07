#include "NodeMaskRenderLogicTr.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/OffscreenRenderLogic.h"

#include "Rendering/Logic/FrameRendering/FrameRenderLogic.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
NodeMaskRenderLogicTr::NodeMaskRenderLogicTr        ()
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
        ctx->GetRenderLogic()->DrawNode( ctx->GetRenderer(), node );
    }
    else
    {
        auto renderer = ctx->GetRenderer();
        auto rtAllocator    = ctx->GetRenderTargetAllocator();

        ctx->GetRenderLogic()->DrawNodeOnly( renderer, node );

        // GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        // GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        renderer->Enable( rt );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        auto effect = node->GetNodeEffect();

        auto maskIdxVal = std::static_pointer_cast< ValueInt >( effect->GetValue( "bgIdx" ) );
        auto fgIdxVal = std::static_pointer_cast< ValueInt >( effect->GetValue( "fgIdx" ) );
        auto alphaVal = effect->GetValue( "alpha" );

        auto maskIdx= maskIdxVal->GetValue();
        auto fgIdx = fgIdxVal->GetValue();

        assert( maskIdx == 0 || maskIdx == 1 );
        assert( fgIdx == 0 || fgIdx == 1 );
        assert( maskIdx != fgIdx );

        // MASK
        ctx->GetRenderLogic()->RenderNode( renderer, node->GetChild( maskIdx ) );

        //GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        //GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        // FOREGROUND
        ctx->GetRenderLogic()->RenderNode( renderer, node->GetChild( fgIdx ) ); 

        //GetOffscreenRenderLogic()->DrawAMTopTwoRenderTargets( renderer, alphaVal.get() );
    
        //GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        //GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

        //GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        ctx->GetRenderLogic()->RenderChildren( renderer, node, 2 );
    }
}

} //bv
