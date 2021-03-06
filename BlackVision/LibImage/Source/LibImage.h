#pragma once

#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

namespace bv { namespace image
{

enum class ImageFormat : int
{
    IF_A32FR32FG32FB32F = 0 ,
    IF_R32FG32FB32F         ,
    IF_A32F                 ,
    IF_A8R8G8B8             ,
    IF_R8G8B8               ,
    IF_A8                   ,
    IF_L8                   ,
    // All below this line are always converted to floating point texture
    IF_A16R16G16B16         , 
    IF_R16G16B16            ,
    IF_A16                  ,
    IF_TOTAL
};

struct ImageProperties
{
    UInt32          width;
    UInt32          height;
    ImageFormat     format;
    std::string     error;
};

enum class BlurType : int
{
    BT_BOX = 0,
    BT_TRIANGLE,
    BT_GAUSSIAN
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

ImageProperties         GetImageProps   ( const std::string & imageFilePath );

 // RAW pointer version
char *                  LoadImageImpl   ( const std::string & filePath, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum, bool loadFromMemory = true );
char *                  LoadRAWImageImpl( const std::string & filePath, SizeType * size );

bool                    SaveBMPImageImpl( const std::string & filePath, const char * data, UInt32 width, UInt32 height, UInt32 bpp );
void                    SaveRAWImageImpl( const std::string & filePath, const char * data, SizeType size );

MemoryChunkConstPtr     SwapChannels    ( const MemoryChunkConstPtr & in, UInt32 bpp, UInt32 b, UInt32 g, UInt32 r, UInt32 a);
MemoryChunkConstPtr     SaveTGAToHandle ( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp );

MemoryChunkConstPtr     AddImages       ( const MemoryChunkConstPtr & in1, const MemoryChunkConstPtr & in2 );

char *                  ResizeImpl          ( const char * in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 newWidth, UInt32 newHeight, FilterType ft );
char *                  MakeThumbnailImpl   ( const char * in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 maxSize );
char *                  BlurImageImpl       ( const char * data, UInt32 width, UInt32 height, UInt32 bpp, Float32 blurSize, BlurType filter );
char *                  BoxBlurImage        ( const char * data, UInt32 width, UInt32 height, UInt32 bpp, Float32 blurSize );
char *                  FlipHorizontalImpl  ( const char * data, UInt32 width, UInt32 height, UInt32 bpp );
char *                  FlipVerticalImpl    ( const char * data, UInt32 width, UInt32 height, UInt32 bpp );

// MemoryChunk version
MemoryChunkConstPtr     LoadImage       ( const std::string & filePath, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum, bool loadFromMemory = true );
MemoryChunkConstPtr     LoadRAWImage    ( const std::string & filePath );

bool                    SaveBMPImage    ( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp );
void                    SaveRAWImage    ( const std::string & filePath, MemoryChunkConstPtr data);

MemoryChunkConstPtr     Resize          ( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 newWidth, UInt32 newHeight, FilterType ft );
MemoryChunkConstPtr     MakeThumbnai    ( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 maxSize );
MemoryChunkConstPtr     BlurImage       ( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, Float32 blurSize, BlurType filter );
MemoryChunkConstPtr     FlipHorizontal  ( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp );
MemoryChunkConstPtr     FlipVertical    ( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp );

} // image
} // bv
