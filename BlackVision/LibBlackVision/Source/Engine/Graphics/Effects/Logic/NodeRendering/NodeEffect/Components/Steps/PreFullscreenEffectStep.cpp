#include "stdafx.h"

#include "PreFullscreenEffectStep.h"


namespace bv { namespace nrl {

// **************************
//
PreFullscreenEffectStep::PreFullscreenEffectStep      ( RenderComponentStatePtr initialState )
    : Parent( initialState )
{
}

// **************************
// Should be called only if (!IsIdle( nodeRepr ))
// It is assumed that the owner of this class has already called ReadInputState
const RenderedData *   PreFullscreenEffectStep::Apply ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // Basic implementation of Template Pattern
    assert( !IsIdle( nodeRepr ) );

    // 1. Allocate intermediate render targets if necessary
    AllocateRenderTargets   ( ctx );

    // 2. Call subclass implementation
    auto res = ApplyImpl    ( nodeRepr, ctx );

    return res;
}

// **************************
//
void                    PreFullscreenEffectStep::ReadInputState        ()
{
    // Default implementation is empty
}

// **************************
//
void                    PreFullscreenEffectStep::AllocateRenderTargets ( RenderContext * ctx )
{
    // Default implementation is empty
    { ctx; }
}

} // nrl
} // bv

