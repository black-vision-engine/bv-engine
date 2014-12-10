#pragma once

#include <string>
#include "Core/MemoryChunk.h"

namespace bv { namespace model {

class TextureHelper
{
public:
    static MemoryChunkConstPtr  LoadImg ( const std::string& filePath, int* width, int* heigth, int* bpp, bool loadFromMemory = true );
	static MemoryChunkConstPtr  LoadRAW ( const std::string& filePath );
    static void                 WriteBMP( const std::string& filePath, MemoryChunkConstPtr data, int width, int height, int bpp );
	static void                 WriteRAW( const std::string& filePath, MemoryChunkConstPtr data );

    static MemoryChunkConstPtr  Blur    ( MemoryChunkConstPtr data, int width, int height, int bpp, int blurSize );
};

} // model
} // bv