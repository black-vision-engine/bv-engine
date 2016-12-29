#include "NFullscreenEffectStep.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectStep::NFullscreenEffectStep    ( NFullscreenEffectComponentStatePtr initialState )
    : Parent( initialState )
{
}

// **************************
// Should be called only if (!IsIdle())
// It is assumed that the owner of this class has already called ReadInputState
void    NFullscreenEffectStep::Apply            ( NRenderContext * ctx, const NRenderedData * input )
{
    // 1. Call subclass implementation
    ApplyImpl           ( ctx, input );

    // 2. Free intermediate render targets if necessary
    FreeRenderTargets   ( ctx, input );
}

// **************************
//
void    NFullscreenEffectStep::ReadInputState   ()
{
    // Default implementation is empty
}

// **************************
//
void    NFullscreenEffectStep::FreeRenderTargets( NRenderContext * ctx, const NRenderedData * input )
{
    // Default implementation is empty
    { ctx; input; }
}

} // nrl
} // bv

