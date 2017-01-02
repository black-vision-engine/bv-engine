#include "stdafx.h"

#include "NFullscreenEffectInstance.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectInstance::~NFullscreenEffectInstance           ()
{
}

// **************************
//
void    NFullscreenEffectInstance::Render                       ( NRenderContext * ctx, const NRenderedData & dynamicInput )
{
    SyncDynamicInput( dynamicInput );

    Render( ctx );
}

} // nrl
} // bv
