#include "LightScatteringRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/OffscreenRenderLogic.h"

#include "Rendering/Logic/RenderLogic.h"

#include "Engine/Graphics/Effects/LightScatteringEffect.h"


namespace bv {

// *********************************
//
LightScatteringRenderLogic::LightScatteringRenderLogic         ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
    , m_effect( new LightScatteringEffect() )
{
}

// *********************************
//
LightScatteringRenderLogic::~LightScatteringRenderLogic        ()
{
}

// *********************************
//
void    LightScatteringRenderLogic::RenderNode                  ( Renderer * renderer, SceneNode * node )
{
    if( node->NumChildNodes() < 2 )
    {
        GetRenderLogic()->DrawNode( renderer, node );
    }
    else
    {
        GetRenderLogic()->DrawNodeOnly( renderer, node );

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
        GetRenderLogic()->RenderNode( renderer, node->GetChild( maskIdx ) );

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        // FOREGROUND
        GetRenderLogic()->RenderNode( renderer, node->GetChild( fgIdx ) ); 

        GetOffscreenRenderLogic()->DrawAMTopTwoRenderTargets( renderer, alphaVal.get() );
    
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        GetRenderLogic()->DrawChildren( renderer, node, 2 );
    }
}

} //bv
