#include "stdafx.h"

#include "BoundingBoxPostFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"
#include "Engine/Graphics/SceneGraph/Lines.h"

#include "Engine/Graphics/Effects/BoundingBoxEffect.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Models/BVProjectTools.h"

#include "Engine/Types/Values/ValuesFactory.h"

namespace bv {

BoundingBoxEffect *   BoundingBoxPostFullscreenEffectLogic::m_effect = new BoundingBoxEffect();


// *********************************
//
BoundingBoxPostFullscreenEffectLogic::BoundingBoxPostFullscreenEffectLogic          ()
{
    m_colorValue = ValuesFactory::CreateValueVec4( "color" );
    m_colorValue->SetValue( glm::vec4( 1, 1, 1, 1 ) );
}

// *********************************
//
BoundingBoxPostFullscreenEffectLogic::~BoundingBoxPostFullscreenEffectLogic         ()
{
    //delete m_effect;
}

// *********************************
//
void                        BoundingBoxPostFullscreenEffectLogic::Render          ( SceneNode * node, RenderLogicContext * ctx )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );

    auto obj = Cast< bv::RenderableEntityWithBoundingBox * >( node->GetTransformable() );

    assert( obj );

    auto box = Cast< bv::Lines * >( obj->GetBoundingBox() );

    box->SetWidth( 5.f );

    auto pass = m_effect->GetPass( 0 );

    auto param = Cast< ShaderParamVec4 * >( pass->GetPixelShader()->GetParameters()->AccessParam( "color" ) );
    param->SetValue( m_colorValue->GetValue() );

    renderer( ctx )->Enable( pass, box );

    renderer( ctx )->DrawRenderable( box );
}

// *********************************
//
std::vector< IValuePtr >    BoundingBoxPostFullscreenEffectLogic::GetValues       () const
{
    std::vector< IValuePtr > res( 1 );
    res[ 0 ] = m_colorValue;

    return res;
}

} //bv
