#include "DefaultEffectRenderLogic.h"

#include "Rendering/Utils/RenderLogicContext.h"


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
    logic( ctx )->DrawNode( renderer( ctx ), node, ctx );
}

} //bv
