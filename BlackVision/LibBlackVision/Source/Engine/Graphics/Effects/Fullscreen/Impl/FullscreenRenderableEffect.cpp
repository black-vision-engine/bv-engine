#include "stdafx.h"

#include "FullscreenRenderableEffect.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// **************************
//
FullscreenRenderableEffect::FullscreenRenderableEffect  ( RenderablePass * pass )
{
    AddPass( pass );
}

} //bv
