#pragma once

#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

namespace bv { namespace image
{

enum class ImageFormat : int
{
    IF_A8R8G8B8 = 0	,
    IF_R8G8B8       ,
    IF_A8           ,
    IF_TOTAL
};

struct ImageProperties
{
	UInt32			width;
	UInt32			height;
	ImageFormat		format;
	std::string		error;
};

	
ImageProperties			GetImageProps	( const std::string & imageFilePath );

MemoryChunkConstPtr		LoadImage		( const std::string & filePath, unsigned int * width, unsigned int * heigth, unsigned int * bpp, bool loadFromMemory = true );
MemoryChunkConstPtr		LoadRAWImage	( const std::string & filePath );
bool					SaveBMPImage	( const std::string & filePath, MemoryChunkConstPtr data, unsigned int width, unsigned int height, unsigned int bpp );
void					SaveRAWImage	( const std::string & filePath, MemoryChunkConstPtr data );

MemoryChunkConstPtr		BlurImage		( MemoryChunkConstPtr data, unsigned int width, unsigned int height, unsigned int bpp, unsigned int blurSize );

} // image
} // bv