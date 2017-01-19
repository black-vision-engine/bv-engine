#include "stdafx.h"

#include "OutputScreenshot.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputScreenshot::OutputScreenshot( unsigned int width, unsigned int height )
    : OutputInstance( width, height )
{
}

// *********************************
//
void    OutputScreenshot::ProcessFrameData  ( NRenderContext * ctx, RenderResult * result )
{
    { ctx; result; }
    assert( false );

    // FIXME: nrl - implement
}

} //nrl
} //bv
