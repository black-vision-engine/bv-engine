#include "DefaultEffectRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/Profiler/HerarchicalProfiler.h"


namespace bv {

// *********************************
//
DefaultEffectRenderLogic::DefaultEffectRenderLogic  ()
{
}

// *********************************
//
DefaultEffectRenderLogic::~DefaultEffectRenderLogic ()
{
}

// *********************************
//
void    DefaultEffectRenderLogic::RenderNode        ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic )
{
    DrawNode( renderer, node, offscreenRenderLogic );
}

} //bv
