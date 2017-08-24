#include "stdafx.h"

#include "DefaultDepthFinalizeStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/DepthRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {


// **************************
//
DefaultDepthFinalizeStep::DefaultDepthFinalizeStep          ()
    : Parent( nullptr )
{}

// **************************
//
void        DefaultDepthFinalizeStep::Apply                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );

    DepthRenderLogic::RenderQueued( nodeRepr, ctx );
}

// **************************
//
bool        DefaultDepthFinalizeStep::IsIdle                    ( SceneNodeRepr * ) const
{
    return false;
}



} // bv
