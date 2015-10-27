#include "NodeMaskRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/OffscreenRenderLogic.h"

#include "Rendering/Logic/RenderLogic.h"


namespace bv {

// *********************************
//
NodeMaskRenderLogic::NodeMaskRenderLogic         ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
{
}

// *********************************
//
NodeMaskRenderLogic::~NodeMaskRenderLogic        ()
{
}

// *********************************
//
void    NodeMaskRenderLogic::RenderNode                  ( Renderer * renderer, SceneNode * node )
{
    if( node->NumChildNodes() < 2 )
    {
        GetRenderLogic()->DrawNodeTM( renderer, node );
    }
    else
    {
        GetRenderLogic()->DrawNodeOnlyTM( renderer, node );

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

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
        GetRenderLogic()->RenderNodeTM( renderer, node->GetChild( maskIdx ) );

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        // FOREGROUND
        GetRenderLogic()->RenderNodeTM( renderer, node->GetChild( fgIdx ) ); 

        GetOffscreenRenderLogic()->DrawAMTopTwoRenderTargets( renderer, alphaVal.get() );
    
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        GetRenderLogic()->DrawChildrenTM( renderer, node, 2 );
    }
}

} //bv
