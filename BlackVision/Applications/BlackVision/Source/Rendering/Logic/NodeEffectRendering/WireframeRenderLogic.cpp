#include "WireframeRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Logic/RenderLogic.h"


namespace bv {

// *********************************
//
WireframeRenderLogic::WireframeRenderLogic  ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
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
    EnableWireframeEffect( renderer );

    DrawWirefreameNodeOnly( renderer, node );
    DrawWireframeChildren( renderer, node );

    DisableWireframeEffect( renderer );
}

// *********************************
//
void    WireframeRenderLogic::EnableWireframeEffect       ( Renderer * renderer )
{
    { renderer; }
    // FIXME: implement
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

// *********************************
//
void    WireframeRenderLogic::DisableWireframeEffect      ( Renderer * renderer )
{
    { renderer; }
    // FIXME: implement
}

} //bv
