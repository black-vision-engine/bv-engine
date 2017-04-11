#include "stdafx.h"

#include "FullscreenEffectStep.h"


namespace bv { 

// **************************
//
FullscreenEffectStep::FullscreenEffectStep    ( FullscreenEffectComponentStatePtr initialState )
    : Parent( initialState )
{
}

// **************************
// Should be called only if (!IsIdle())
// It is assumed that the owner of this class has already called ReadInputState
void    FullscreenEffectStep::Apply            ( RenderContext * ctx, const RenderedData * input )
{
    // 1. Call subclass implementation
    ApplyImpl           ( ctx, input );

    // 2. Free intermediate render targets if necessary
    FreeRenderTargets   ( ctx, input );
}

// **************************
//
void    FullscreenEffectStep::ReadInputState   ()
{
    // Default implementation is empty
}

// **************************
//
void    FullscreenEffectStep::FreeRenderTargets( RenderContext * ctx, const RenderedData * input )
{
    // Default implementation is empty
    { ctx; input; }
}


} // bv

