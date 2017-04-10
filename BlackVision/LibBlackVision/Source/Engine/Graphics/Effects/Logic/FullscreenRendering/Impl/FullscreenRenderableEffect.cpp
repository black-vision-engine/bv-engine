#include "stdafx.h"

#include "FullscreenRenderableEffect.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenRenderableEffect::NFullscreenRenderableEffect  ( RenderablePass * pass )
{
    AddPass( pass );
}

} //nrl
} //bv
