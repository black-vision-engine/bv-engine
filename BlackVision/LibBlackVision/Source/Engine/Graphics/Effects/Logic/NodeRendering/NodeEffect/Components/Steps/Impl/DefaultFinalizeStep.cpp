#include "stdafx.h"

#include "DefaultFinalizeStep.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"


namespace bv { 

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


} // bv
