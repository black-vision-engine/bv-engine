#include "stdafx.h"

#include "NodeMaskFinalizeStep.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
NodeMaskFinalizeStep::NodeMaskFinalizeStep        ()
    : Parent( nullptr )
{
}

// **************************
//
void    NodeMaskFinalizeStep::Apply		        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    NodeRenderLogic::RenderChildren( nodeRepr, ctx, 2 );
}

// **************************
//
bool    NodeMaskFinalizeStep::IsIdle               ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() <= 2;
}

} // nrl
} // bv
