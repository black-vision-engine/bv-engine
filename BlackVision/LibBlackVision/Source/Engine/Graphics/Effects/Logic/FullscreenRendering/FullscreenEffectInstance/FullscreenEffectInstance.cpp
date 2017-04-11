#include "stdafx.h"

#include "FullscreenEffectInstance.h"


namespace bv { 

// **************************
//
FullscreenEffectInstance::~FullscreenEffectInstance           ()
{
}

// **************************
//
void    FullscreenEffectInstance::Render                       ( RenderContext * ctx, const RenderedData & dynamicInput )
{
    SyncDynamicInput( dynamicInput );

    Render( ctx );
}


} // bv
