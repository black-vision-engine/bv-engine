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

enum class FilterType : int
{
	FT_BOX = 0,
	FT_BILINEAR,
	FT_B_SPLINE,
	FT_BICUBIC,
	FT_CATMULL_ROM,
	FT_LANCZOS
};
	
ImageProperties			GetImageProps	( const std::string & imageFilePath );

MemoryChunkConstPtr		LoadImage		( const std::string & filePath, UInt32 * width, UInt32 * heigth, UInt32 * bpp, bool loadFromMemory = true );
MemoryChunkConstPtr		LoadRAWImage	( const std::string & filePath );
bool					SaveBMPImage	( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp );
void					SaveRAWImage	( const std::string & filePath, MemoryChunkConstPtr data );

MemoryChunkConstPtr		Resize			( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 newWidth, UInt32 newHeight, FilterType ft );
MemoryChunkConstPtr		BlurImage		( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, UInt32 blurSize );

} // image
} // bv