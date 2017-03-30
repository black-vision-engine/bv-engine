#include "stdafx.h"

#include "NDefaultFinalizeStep.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
NDefaultFinalizeStep::NDefaultFinalizeStep      ()
    : Parent( nullptr )
{
}

// **************************
//
void    NDefaultFinalizeStep::Apply		        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    // Default rendering
    NNodeRenderLogic::Render( nodeRepr, ctx );
}

} // nrl
} // bv
