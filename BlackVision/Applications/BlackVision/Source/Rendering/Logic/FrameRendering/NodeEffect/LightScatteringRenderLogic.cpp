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

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/RenderLogicContext.h"


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
void    LightScatteringRenderLogic::RenderNode                  ( SceneNode * node, RenderLogicContext * ctx )
    
{
    //GetRenderLogic()->DrawNode( renderer, node );

    renderer( ctx )->Disable( allocator( ctx )->Top() );    

    auto renderer       = ctx->GetRenderer();
    auto rtAllocator    = ctx->GetRenderTargetAllocator();

    auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
    renderer->Enable( rt );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    logic( ctx )->DrawNode( renderer, node );

    renderer->Disable( rt );
    rtAllocator->Free();

    //

    auto texture = rt->ColorTexture( 0 );

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

    renderer->Enable( allocator( ctx )->Top() );   

    renderer->Draw(ts);
}

} //bv
