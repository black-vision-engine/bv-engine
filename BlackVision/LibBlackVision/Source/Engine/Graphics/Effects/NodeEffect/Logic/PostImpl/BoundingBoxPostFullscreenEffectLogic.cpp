#include "stdafx.h"

#include "BoundingBoxPostFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"

#include "Engine/Graphics/Effects/BoundingBoxEffect.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"

#include "Engine/Models/BVProjectTools.h"

namespace bv {

BoundingBoxEffect *   BoundingBoxPostFullscreenEffectLogic::m_effect = new BoundingBoxEffect();


// *********************************
//
BoundingBoxPostFullscreenEffectLogic::BoundingBoxPostFullscreenEffectLogic          ()
    //: m_effect( new BoundingBoxEffect() )
{
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

    //renderer( ctx )->DrawRenderable( obj );

    auto box = obj->GetBoundingBox();

    renderer( ctx )->Enable( m_effect->GetPass( 0 ), obj ); // FIXME for some reasons box results in bad transformation

    renderer( ctx )->DrawRenderable( box );
}

// *********************************
//
std::vector< IValuePtr >    BoundingBoxPostFullscreenEffectLogic::GetValues       () const
{
    return std::vector< IValuePtr >();
}

} //bv
