#pragma once 

#include "LibImage.h"

namespace bv { namespace effect
{


// *********************************
// Bluring image using GPU
MemoryChunkConstPtr		GLBlurImage( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bbp );


} // effect
} // bv