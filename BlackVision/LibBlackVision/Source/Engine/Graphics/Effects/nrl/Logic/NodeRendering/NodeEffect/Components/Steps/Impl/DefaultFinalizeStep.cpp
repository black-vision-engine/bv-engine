#include "stdafx.h"

#include "DefaultFinalizeStep.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
DefaultFinalizeStep::DefaultFinalizeStep      ()
    : Parent( nullptr )
{
}

// **************************
//
void    DefaultFinalizeStep::Apply		        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // Default rendering
    NodeRenderLogic::Render( nodeRepr, ctx );
}

} // nrl
} // bv
