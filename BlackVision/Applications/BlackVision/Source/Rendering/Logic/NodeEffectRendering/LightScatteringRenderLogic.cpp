#include "LightScatteringRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Rendering/OffscreenRenderLogic.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

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
    //GetRenderLogic()->DrawNode( renderer, node );

    //if( node->NumChildNodes() < 2 )
    //{
    //    GetRenderLogic()->DrawNode( renderer, node );
    //}
    //else
    //{
    //    GetRenderLogic()->DrawNodeOnly( renderer, node );

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        GetRenderLogic()->DrawNode( renderer, node );

        GetOffscreenRenderLogic()->DisableTopRenderTarget( renderer );

        auto effect = node->GetNodeEffect();
        m_effect->SetExposureVal( effect->GetValue( "exposure" ).get() );
        m_effect->SetWeightVal( effect->GetValue( "weight" ).get() );
        m_effect->SetDecayVal( effect->GetValue( "decay" ).get() );
        m_effect->SetDensityVal( effect->GetValue( "density" ).get() );
        m_effect->SetLightPositionOnScreenVal( effect->GetValue( "lightPositionOnScreen" ).get() );
        m_effect->SetNumSamplesVal( effect->GetValue( "numSamples" ).get() );

        auto texture = GetOffscreenRenderLogic()->GetColorTextureAt( -1 );

        m_effect->AddTexture( std::const_pointer_cast< Texture2D >( texture ) );

        renderer->Enable( m_effect->GetPass( 0 ), static_cast< bv::RenderableEntity * >( node->GetTransformable() ) );

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        //renderer->Draw( GetOffscreenRenderLogic()->CurDisplayRenderTargetData().quad );
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        //GetRenderLogic()->RenderFrame( renderer, nullptr );

    //    auto maskIdxVal = std::static_pointer_cast< ValueInt >( effect->GetValue( "bgIdx" ) );
    //    auto fgIdxVal = std::static_pointer_cast< ValueInt >( effect->GetValue( "fgIdx" ) );
    //    auto alphaVal = effect->GetValue( "alpha" );

    //    auto maskIdx= maskIdxVal->GetValue();
    //    auto fgIdx = fgIdxVal->GetValue();

    //    assert( maskIdx == 0 || maskIdx == 1 );
    //    assert( fgIdx == 0 || fgIdx == 1 );
    //    assert( maskIdx != fgIdx );

    //    // MASK
    //    GetRenderLogic()->RenderNode( renderer, node->GetChild( maskIdx ) );

    //    GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
    //    GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
    //    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    //    renderer->ClearBuffers();

    //    // FOREGROUND
    //    GetRenderLogic()->RenderNode( renderer, node->GetChild( fgIdx ) ); 

    //    GetOffscreenRenderLogic()->DrawAMTopTwoRenderTargets( renderer, alphaVal.get() );
    //
    //    GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
    //    GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

    //    GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

    //    GetRenderLogic()->DrawChildren( renderer, node, 2 );
    //}
}

} //bv
