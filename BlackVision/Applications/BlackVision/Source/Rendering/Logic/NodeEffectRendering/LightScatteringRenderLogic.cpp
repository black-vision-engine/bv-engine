#include "LightScatteringRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Rendering/OffscreenRenderLogic.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Rendering/Logic/RenderLogic.h"

#include "Engine/Graphics/Effects/LightScatteringEffect.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"


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


        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        GetRenderLogic()->DrawNode( renderer, node );

        auto texture = GetOffscreenRenderLogic()->GetColorTextureAt( -1 );

        auto effect = node->GetNodeEffect();
        m_effect->SetExposureVal( effect->GetValue( "exposure" ).get() );
        m_effect->SetWeightVal( effect->GetValue( "weight" ).get() );
        m_effect->SetDecayVal( effect->GetValue( "decay" ).get() );
        m_effect->SetDensityVal( effect->GetValue( "density" ).get() );
        m_effect->SetLightPositionOnScreenVal( effect->GetValue( "lightPositionOnScreen" ).get() );
        m_effect->SetNumSamplesVal( effect->GetValue( "numSamples" ).get() );

        auto ts = MainDisplayTarget::CreateAuxRect( std::const_pointer_cast< Texture2D >( texture ) );

        std::vector< bv::Transform > vec;
        vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

        ts->SetWorldTransforms( vec );

        m_effect->AddTexture( std::const_pointer_cast< Texture2D >( texture ) );

        ts->SetRenderableEffect( RenderableEffectPtr( m_effect ) );

        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        renderer->Draw(ts);
}

} //bv
