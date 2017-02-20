#include "stdafx.h"

#include "NNodeMaskFinalizeStep.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
NNodeMaskFinalizeStep::NNodeMaskFinalizeStep        ()
    : Parent( nullptr )
{
}

// **************************
//
void    NNodeMaskFinalizeStep::Apply		        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    NNodeRenderLogic::RenderChildren( nodeRepr, ctx, 2 );
}

// **************************
//
bool    NNodeMaskFinalizeStep::IsIdle               ( SceneNodeRepr * nodeRepr ) const
{
    return nodeRepr->NumChildNodes() <= 2;
}

} // nrl
} // bv
