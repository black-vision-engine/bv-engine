#include "DefaultEffectRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Logic/RenderLogic.h"


namespace bv {

// *********************************
//
DefaultEffectRenderLogic::DefaultEffectRenderLogic  ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
{
}

// *********************************
//
DefaultEffectRenderLogic::~DefaultEffectRenderLogic ()
{
}

// *********************************
//
void    DefaultEffectRenderLogic::RenderNode        ( Renderer * renderer, SceneNode * node )
{
    GetRenderLogic()->DrawNode( renderer, node );
}

} //bv
