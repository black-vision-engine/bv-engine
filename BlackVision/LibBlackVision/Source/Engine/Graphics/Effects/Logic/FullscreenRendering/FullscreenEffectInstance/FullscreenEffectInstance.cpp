#include "stdafx.h"

#include "FullscreenEffectInstance.h"


namespace bv { namespace nrl {

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

} // nrl
} // bv
