#include "WireframeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/WireframeEffect.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
WireframeRenderLogic::WireframeRenderLogic  ()
    : m_effect( new WireframeEffect() )
{
}

// *********************************
//
WireframeRenderLogic::~WireframeRenderLogic ()
{
    delete m_effect;
}

// *********************************
//
void    WireframeRenderLogic::RenderNode      ( SceneNode * node, RenderLogicContext * ctx )
{
    logic( ctx )->DrawNode( renderer( ctx ), node, ctx );

    RenderWireframeOverlay( renderer( ctx ), node );
}

// *********************************
//
void    WireframeRenderLogic::RenderWireframeOverlay    ( Renderer * renderer, SceneNode * node )
{
    EnableWireframeEffect( renderer, node );

    DrawWirefreameNodeOnly( renderer, node );
    // DrawWireframeChildren( renderer, node );
}

// *********************************
//
void    WireframeRenderLogic::EnableWireframeEffect       ( Renderer * renderer, SceneNode * node )
{
    // FIXME: add IValues to the effect
    //auto pass = m_effect->GetPass( 0 );

    renderer->Enable( m_effect->GetPass( 0 ), static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    WireframeRenderLogic::DrawWirefreameNodeOnly      ( Renderer * renderer, SceneNode * node )
{
    auto renderable = static_cast<bv::RenderableEntity *>( node->GetTransformable() );

    renderer->DrawRenderable( renderable );
}

// *********************************
//
void    WireframeRenderLogic::DrawWireframeChildren       ( Renderer * renderer, SceneNode * node )
{
    for ( unsigned int i = 0; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        DrawWirefreameNodeOnly( renderer, node->GetChild( i ) ); 
        DrawWireframeChildren( renderer, node->GetChild( i ) );
    }
}

} //bv
