#include "WireframeRenderLogicTr.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Logic/FrameRendering/FrameRenderLogic.h"

#include "Engine/Graphics/Effects/WireframeEffect.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
WireframeRenderLogicTr::WireframeRenderLogicTr  ()
    : m_effect( new WireframeEffect() )
{
}

// *********************************
//
WireframeRenderLogicTr::~WireframeRenderLogicTr ()
{
    delete m_effect;
}

// *********************************
//
void    WireframeRenderLogicTr::RenderNode      ( SceneNode * node, RenderLogicContext * ctx )
{
    ctx->GetRenderLogic()->DrawNode( ctx->GetRenderer(), node );

    RenderWireframeOverlay( ctx->GetRenderer(), node );
}

// *********************************
//
void    WireframeRenderLogicTr::RenderWireframeOverlay    ( Renderer * renderer, SceneNode * node )
{
    EnableWireframeEffect( renderer, node );

    DrawWirefreameNodeOnly( renderer, node );
    // DrawWireframeChildren( renderer, node );
}

// *********************************
//
void    WireframeRenderLogicTr::EnableWireframeEffect       ( Renderer * renderer, SceneNode * node )
{
    // FIXME: add IValues to the effect
    //auto pass = m_effect->GetPass( 0 );

    renderer->Enable( m_effect->GetPass( 0 ), static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    WireframeRenderLogicTr::DrawWirefreameNodeOnly      ( Renderer * renderer, SceneNode * node )
{
    auto renderable = static_cast<bv::RenderableEntity *>( node->GetTransformable() );

    renderer->DrawRenderable( renderable );
}

// *********************************
//
void    WireframeRenderLogicTr::DrawWireframeChildren       ( Renderer * renderer, SceneNode * node )
{
    for ( unsigned int i = 0; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        DrawWirefreameNodeOnly( renderer, node->GetChild( i ) ); 
        DrawWireframeChildren( renderer, node->GetChild( i ) );
    }
}

} //bv
