#include "WireframeRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Logic/RenderLogic.h"

#include "Engine/Graphics/Effects/WireframeEffect.h"


namespace bv {

// *********************************
//
WireframeRenderLogic::WireframeRenderLogic  ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
    , m_effect( new WireframeEffect() )
{
}

// *********************************
//
WireframeRenderLogic::~WireframeRenderLogic ()
{
}

// *********************************
//
void    WireframeRenderLogic::RenderNode                ( Renderer * renderer, SceneNode * node )
{
    GetRenderLogic()->DrawNode( renderer, node );

    RenderWireframeOverlay( renderer, node );
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
