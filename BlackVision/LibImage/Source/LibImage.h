#pragma once

#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

namespace bv { namespace image
{

enum class ImageFormat : int
{
	IF_A32FR32FG32FB32F = 0	,
	IF_R32FG32FB32F			,
	IF_A32F					,
    IF_A8R8G8B8				,
    IF_R8G8B8				,
    IF_A8					,
    IF_L8					,
	// All below this line are always converted to floating point texture
	IF_A16R16G16B16			, 
	IF_R16G16B16			,
	IF_A16					,
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

 // RAW pointer version
char *		            LoadImageImpl	( const std::string & filePath, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum, bool loadFromMemory = true );
char *                  LoadRAWImageImpl( const std::string & filePath, SizeType * size );

bool					SaveBMPImageImpl( const std::string & filePath, const char * data, UInt32 width, UInt32 height, UInt32 bpp );
void					SaveRAWImageImpl( const std::string & filePath, const char * data, SizeType size );

MemoryChunkConstPtr     SaveTGAToHandle ( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp );

char *		            ResizeImpl		( const char * in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 newWidth, UInt32 newHeight, FilterType ft );
char *		            BlurImageImpl	( const char * data, UInt32 width, UInt32 height, UInt32 bpp, UInt32 blurSize );

// MemoryChunk version
MemoryChunkConstPtr		LoadImage		( const std::string & filePath, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum, bool loadFromMemory = true );
MemoryChunkConstPtr		LoadRAWImage	( const std::string & filePath );

bool					SaveBMPImage	( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp );
void					SaveRAWImage	( const std::string & filePath, MemoryChunkConstPtr data);

MemoryChunkConstPtr		Resize			( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 newWidth, UInt32 newHeight, FilterType ft );
MemoryChunkConstPtr		BlurImage		( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, UInt32 blurSize );

} // image
} // bv
