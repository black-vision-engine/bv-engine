#include "WireframeRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

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
void    WireframeRenderLogic::RenderNode        ( Renderer * renderer, SceneNode * node )
{
    GetRenderLogic()->DrawNode( renderer, node );
}

} //bv
