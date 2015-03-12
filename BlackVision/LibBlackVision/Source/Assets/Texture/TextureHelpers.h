#pragma once

#include <string>
#include "Memory/MemoryChunk.h"

namespace bv { namespace model {

class TextureHelper
{
public:

    static MemoryChunkConstPtr  LoadImg ( const std::string & filePath, unsigned int * width, unsigned int * heigth, unsigned int * bpp, bool loadFromMemory = true );
	static MemoryChunkConstPtr  LoadRAW ( const std::string & filePath );
    static void                 WriteBMP( const std::string & filePath, MemoryChunkConstPtr data, unsigned int width, unsigned int height, unsigned int bpp );
	static void                 WriteRAW( const std::string & filePath, MemoryChunkConstPtr data );

    static MemoryChunkConstPtr  Blur    ( MemoryChunkConstPtr data, unsigned int width, unsigned int height, unsigned int bpp, unsigned int blurSize );
};

} // model
} // bv
