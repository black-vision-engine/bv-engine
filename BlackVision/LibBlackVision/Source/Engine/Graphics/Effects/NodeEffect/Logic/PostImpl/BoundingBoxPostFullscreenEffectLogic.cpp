#include "stdafx.h"

#include "BoundingBoxPostFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/BoundingBoxEffect.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
BoundingBoxPostFullscreenEffectLogic::BoundingBoxPostFullscreenEffectLogic          ()
    : m_effect( new BoundingBoxEffect() )
{
}

// *********************************
//
BoundingBoxPostFullscreenEffectLogic::~BoundingBoxPostFullscreenEffectLogic         ()
{
    delete m_effect;
}

// *********************************
//
void                        BoundingBoxPostFullscreenEffectLogic::Render          ( SceneNode * node, RenderLogicContext * ctx )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );

    node; ctx;

    EnableBoundingBoxEffect( renderer( ctx ), node );
    DrawWirefreameNodeOnly( renderer( ctx ), node );
}

// *********************************
//
std::vector< IValuePtr >    BoundingBoxPostFullscreenEffectLogic::GetValues       () const
{
    return std::vector< IValuePtr >();
}

// *********************************
//
void    BoundingBoxPostFullscreenEffectLogic::EnableBoundingBoxEffect               ( Renderer * renderer, SceneNode * node )
{
    // FIXME: add IValues to the effect
    //auto pass = m_effect->GetPass( 0 );

    renderer->Enable( m_effect->GetPass( 0 ), static_cast< bv::RenderableEntity * >( node->GetTransformable() ) );
}

// *********************************
//
void    BoundingBoxPostFullscreenEffectLogic::DrawWirefreameNodeOnly              ( Renderer * renderer, SceneNode * node )
{
    auto renderable = static_cast< bv::RenderableEntity * >( node->GetTransformable() );

    renderer->DrawRenderable( renderable );
}

} //bv
