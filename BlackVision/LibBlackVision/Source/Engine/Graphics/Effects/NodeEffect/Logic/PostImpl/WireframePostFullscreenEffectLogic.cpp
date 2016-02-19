#include "stdafx.h"

#include "WireframePostFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/WireframeEffect.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
WireframePostFullscreenEffectLogic::WireframePostFullscreenEffectLogic          ()
    : m_effect( new WireframeEffect() )
{
}

// *********************************
//
WireframePostFullscreenEffectLogic::~WireframePostFullscreenEffectLogic         ()
{
    delete m_effect;
}

// *********************************
//
void                        WireframePostFullscreenEffectLogic::Render          ( SceneNode * node, RenderLogicContext * ctx )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );

    EnableWireframeEffect( renderer( ctx ), node );
    DrawWirefreameNodeOnly( renderer( ctx ), node );
}

// *********************************
//
std::vector< IValuePtr >    WireframePostFullscreenEffectLogic::GetValues       () const
{
    return std::vector< IValuePtr >();
}

// *********************************
//
void    WireframePostFullscreenEffectLogic::EnableWireframeEffect               ( Renderer * renderer, SceneNode * node )
{
    // FIXME: add IValues to the effect
    //auto pass = m_effect->GetPass( 0 );

    renderer->Enable( m_effect->GetPass( 0 ), static_cast< bv::RenderableEntity * >( node->GetTransformable() ) );
}

// *********************************
//
void    WireframePostFullscreenEffectLogic::DrawWirefreameNodeOnly              ( Renderer * renderer, SceneNode * node )
{
    auto renderable = static_cast< bv::RenderableEntity * >( node->GetTransformable() );

    renderer->DrawRenderable( renderable );
}

} //bv
