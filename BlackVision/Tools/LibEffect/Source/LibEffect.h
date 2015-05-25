#pragma once 

#include "LibImage.h"

namespace bv
{

class Renderer;

namespace effect
{

extern bv::Renderer * g_renderer;

// *********************************
// Initialize library
void InitializeLibEffect( bv::Renderer * renderer );

// *********************************
// Bluring image using GPU
MemoryChunkConstPtr		GLBlurImage( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 blurLength );


} // effect
} // bv