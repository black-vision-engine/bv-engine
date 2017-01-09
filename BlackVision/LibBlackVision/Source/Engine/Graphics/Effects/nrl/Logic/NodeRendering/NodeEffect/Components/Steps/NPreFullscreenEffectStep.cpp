#include "stdafx.h"

#include "NPreFullscreenEffectStep.h"


namespace bv { namespace nrl {

// **************************
//
NPreFullscreenEffectStep::NPreFullscreenEffectStep      ( NRenderComponentStatePtr initialState )
    : Parent( initialState )
{
}

// **************************
// Should be called only if (!IsIdle( nodeRepr ))
// It is assumed that the owner of this class has already called ReadInputState
const NRenderedData *   NPreFullscreenEffectStep::Apply ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
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
void                    NPreFullscreenEffectStep::ReadInputState        ()
{
    // Default implementation is empty
}

// **************************
//
void                    NPreFullscreenEffectStep::AllocateRenderTargets ( NRenderContext * ctx )
{
    // Default implementation is empty
    { ctx; }
}

} // nrl
} // bv

