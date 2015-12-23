#include "LightScatteringRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Rendering/Utils/OffscreenRenderLogic.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Rendering/Logic/RenderLogic.h"

#include "Engine/Graphics/Effects/LightScatteringEffect.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"


namespace bv {


// *********************************
//
LightScatteringRenderLogic::LightScatteringRenderLogic         ()
    : m_effect( new LightScatteringEffect() )
{
}

// *********************************
//
LightScatteringRenderLogic::~LightScatteringRenderLogic        ()
{
}

// *********************************
//
//void    LightScatteringRenderLogic::RenderNode                  ( Renderer * renderer, SceneNode * node ) // FIXME: default->TDP2015
void    LightScatteringRenderLogic::RenderNode                  ( SceneNode * , RenderLogicContext *   )
    
{
    //GetRenderLogic()->DrawNode( renderer, node );


//        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
//        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
//
//        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
//        renderer->ClearBuffers();
//
//        GetRenderLogic()->DrawNode( renderer, node );
//
//        auto texture = GetOffscreenRenderLogic()->GetColorTextureAt( -1 );
//
//        GetOffscreenRenderLogic()->DisableTopRenderTarget( renderer );
//
//        auto effect = node->GetNodeEffect();
//        m_effect->SetExposureVal( effect->GetValue( "exposure" ).get() );
//        m_effect->SetWeightVal( effect->GetValue( "weight" ).get() );
//        m_effect->SetDecayVal( effect->GetValue( "decay" ).get() );
//        m_effect->SetDensityVal( effect->GetValue( "density" ).get() );
//        m_effect->SetLightPositionOnScreenVal( effect->GetValue( "lightPositionOnScreen" ).get() );
//        m_effect->SetNumSamplesVal( effect->GetValue( "numSamples" ).get() );
//
//        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
//
//        m_effect->AddTexture( std::const_pointer_cast< Texture2D >( texture ) );
//
//        auto re = static_cast< bv::RenderableEntity * >( node->GetTransformable() );
//        
//        auto prevRE = re->GetRenderableEffect();
//
//        re->SetRenderableEffect( RenderableEffectPtr( m_effect ) );
//
//        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
//
//        renderer->Draw( static_cast< bv::RenderableEntity * >( node->GetTransformable() ) );
//
//        re->SetRenderableEffect( prevRE );
}

} //bv
