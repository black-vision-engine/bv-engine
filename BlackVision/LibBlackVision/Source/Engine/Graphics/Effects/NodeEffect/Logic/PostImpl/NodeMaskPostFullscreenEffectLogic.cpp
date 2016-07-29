#include "stdafx.h"

#include "NodeMaskPostFullscreenEffectLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
NodeMaskPostFullscreenEffectLogic::NodeMaskPostFullscreenEffectLogic        ()
{
}

// *********************************
//
void                        NodeMaskPostFullscreenEffectLogic::Render       ( SceneNode * node, RenderLogicContext * ctx )
{
    logic( ctx )->RenderChildren( node, ctx, 2 );
}

// *********************************
//
std::vector< IValuePtr >    NodeMaskPostFullscreenEffectLogic::GetValues    () const
{
    return std::vector< IValuePtr >();
}

} //bv
