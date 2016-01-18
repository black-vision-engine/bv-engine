#include "DefaultEffectRenderLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


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
void    DefaultEffectRenderLogic::RenderNode        ( SceneNode * node, RenderLogicContext * ctx )
{
    logic( ctx )->DrawNode( node, ctx );
}

} //bv
