#include "LibImage.h"
#include "IO/FileIO.h"
#include "System/Path.h"

#include "FreeImage.h"

#include <algorithm>
#include <memory>
#include <cassert>

namespace bv { namespace image
{

struct Float4
{
    Float32 x;
    Float32 y;
    Float32 z;
    Float32 w;

    Float4( Float32 x_, Float32 y_, Float32 z_, Float32 w_ )
        : x( x_ ), y( y_ ), z( z_ ), w( w_ ) {}

    Float4 operator + ( const Float4 & rhs )
    {
        return Float4( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
    }

    Float4 operator - ( const Float4 & rhs )
    {
        return Float4( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
    }

    Float4 operator * ( Float32 f )
    {
        return Float4( x * f, y * f, z * f, w * f );
    }
};

// *********************************
//
ImageProperties GetImageProps( const std::string & imageFilePath )
{
    if( !Path::Exists( imageFilePath ) )
    {
        ImageProperties iprops;
        iprops.error = std::string("File doesn't exist");
        return iprops;
    }

    FREE_IMAGE_FORMAT fiff = FreeImage_GetFileType( imageFilePath.c_str(), 0 );

    if( fiff == FIF_UNKNOWN )
    {
        fiff = FreeImage_GetFIFFromFilename( imageFilePath.c_str() );
        if( fiff == FIF_UNKNOWN )
        {
            ImageProperties iprops;
            iprops.error = std::string( "Unknown file format" );
            return iprops;
        }
    }

    FIBITMAP * bitmap = FreeImage_Load( fiff, imageFilePath.c_str(), FIF_LOAD_NOPIXELS );

    if( bitmap == nullptr )
    {
        ImageProperties iprops;
        iprops.error = std::string("Cannot load file: ") + imageFilePath;
        return iprops;
    }

    auto bbp        = FreeImage_GetBPP( bitmap );
    auto imageType    = FreeImage_GetImageType( bitmap );

    ImageFormat format = bv::image::ImageFormat::IF_TOTAL;

    if( imageType == FIT_BITMAP )
    {
        if( bbp == 32 )
            format = bv::image::ImageFormat::IF_A8R8G8B8;
        else if ( bbp == 24 )
            format = bv::image::ImageFormat::IF_R8G8B8;
        else if( bbp == 8 )
            format = bv::image::ImageFormat::IF_A8;
    }
    else if ( imageType == FIT_FLOAT )
        format = bv::image::ImageFormat::IF_A32F;
    else if ( imageType == FIT_RGBF )
        format = bv::image::ImageFormat::IF_R32FG32FB32F;
    else if ( imageType == FIT_RGBAF )
        format = bv::image::ImageFormat::IF_A32FR32FG32FB32F;
    else if ( imageType == FIT_INT16 )
        format = bv::image::ImageFormat::IF_A16;
    else if ( imageType == FIT_RGB16 )
        format = bv::image::ImageFormat::IF_R16G16B16;
    else if ( imageType == FIT_RGBA16 )
        format = bv::image::ImageFormat::IF_A16R16G16B16;
    else
    {
        ImageProperties iprops;
        iprops.error = std::string("Image format not supported");
        iprops.format = image::ImageFormat::IF_TOTAL;
        iprops.width = 0;
        iprops.height = 0;

        return iprops;
    }

    ImageProperties iprops;

    iprops.format = format;
    iprops.width = ( bv::SizeType )FreeImage_GetWidth( bitmap );
    iprops.height = ( bv::SizeType )FreeImage_GetHeight( bitmap );

    FreeImage_Unload( bitmap );

    return iprops;
}

namespace {

// *********************************
//
FIBITMAP * ConvertToNearestSupported( FIBITMAP * bitmap, UInt32 * bpp, UInt32 * channelNum )
{
    auto bitsPerPixel = FreeImage_GetBPP( bitmap );

    auto imgType = FreeImage_GetImageType( bitmap );

    if( imgType == FIT_BITMAP )
    {
        switch( bitsPerPixel )
        {
        case 32:
        case 24:
        case 16:
            *bpp = 32;
            *channelNum = 4;
            return FreeImage_ConvertTo32Bits( bitmap );
        case 8:
        case 4:
        case 1:
            *bpp = 8;
            *channelNum = 1;
            return FreeImage_ConvertTo8Bits( bitmap );
        }
    }
    else if( imgType < FIT_DOUBLE )
    {
        *bpp = 32;
        *channelNum = 1;
        return FreeImage_ConvertToFloat( bitmap );
    }
    else if( imgType >= FIT_RGB16 && imgType <= FIT_RGBAF )
    {
        *bpp = 128;
        *channelNum = 4;
        return FreeImage_ConvertToRGBAF( bitmap );
    }

    assert( !"Not supported texture format" );
    return nullptr;
}

// ******************************
//
FREE_IMAGE_FILTER ToFIFilter( FilterType ft )
{
    switch ( ft )
    {
    case FilterType::FT_BOX:
        return FREE_IMAGE_FILTER::FILTER_BOX;
    case FilterType::FT_BILINEAR:
        return FREE_IMAGE_FILTER::FILTER_BILINEAR;
    case FilterType::FT_B_SPLINE:
        return FREE_IMAGE_FILTER::FILTER_BSPLINE;
    case FilterType::FT_BICUBIC:
        return FREE_IMAGE_FILTER::FILTER_BICUBIC;
    case FilterType::FT_CATMULL_ROM:
        return FREE_IMAGE_FILTER::FILTER_CATMULLROM;
    case FilterType::FT_LANCZOS:
        return FREE_IMAGE_FILTER::FILTER_LANCZOS3;
    default:
        assert( false && "Unreachable" );
        return FREE_IMAGE_FILTER::FILTER_BOX;
    }
}

// *********************************
//
inline unsigned char GetPixelColor( Int32 x, Int32 y, const char* data, UInt32 width, UInt32 height )
{
    if( x < 0 || x >= ( Int32 )width || y < 0 || y >= ( Int32 )height )
        return 0;

    return data[ 4 * ( x + y * width ) ];
}

// *********************************
//
inline Float4 GetPixelColorFloat4( Int32 x, Int32 y, const char * data, UInt32 width, UInt32 height )
{
    x = x >= 0 && x < ( Int32 )width ? x : 0;
    y = y >= 0 && y < ( Int32 )height ? y : 0;

    const unsigned char * p = (unsigned char *)&data[ 4 * ( x + y * width ) ];

    return Float4( p[ 0 ] / 255.f, p[ 1 ] / 255.f, p[ 2 ] / 255.f, p[ 3 ] / 255.f );
}

// *********************************
//
inline void SetPixelColor( Int32 x, Int32 y, char * data, UInt32 width, UInt32 height, char color )
{
    { height; } // FIXME: suppress unused warning

    memset( &( data[ 4 * ( x + y * width ) ] ), color, 4 );
}

// *********************************
//
inline void SetPixelColorFloat4( Int32 x, Int32 y, char * data, UInt32 width, UInt32 height, const Float4 & color )
{
    { height; } // FIXME: suppress unused warning

    char * p = &data[ 4 * ( x + y * width ) ];

    p[ 0 ] = char( color.x * 255.f );
    p[ 1 ] = char( color.y * 255.f );
    p[ 2 ] = char( color.z * 255.f );
    p[ 3 ] = char( color.w * 255.f );
}

} // anonymous


// *********************************
//
char *                    LoadImageImpl    ( const std::string & filePath, UInt32 * width, UInt32 * heigth, UInt32 * bpp, UInt32 * channelNum, bool loadFromMemory )
{
    FIBITMAP * bitmap = nullptr;

    if( !Path::Exists( filePath ) )
        return nullptr;

    auto size = File::Size( filePath );

    FIMEMORY * memory = nullptr;
    char * pixels = nullptr;

    if( loadFromMemory )
    {
        char * bufToRead = new char[ size ]; 

        SizeType bytes = File::Read( bufToRead, filePath );

        memory = FreeImage_OpenMemory( ( BYTE * ) bufToRead, ( DWORD ) bytes );

        FREE_IMAGE_FORMAT fiff = FreeImage_GetFileTypeFromMemory( memory, 0 );

        bitmap = FreeImage_LoadFromMemory( fiff, memory, BMP_DEFAULT );

        delete[] bufToRead;
    }
    else
    {
        FREE_IMAGE_FORMAT fiff = FreeImage_GetFileType( filePath.c_str(), 0 );

        bitmap = FreeImage_Load( fiff, filePath.c_str(), BMP_DEFAULT );
    }

    if( bitmap )
    {
        auto bitmapcvt = ConvertToNearestSupported( bitmap, bpp, channelNum );

        if( bitmapcvt != bitmap )
        {
            FreeImage_Unload( bitmap );
        }

        if( bitmapcvt )
        {
            *width = FreeImage_GetWidth( bitmapcvt );
            *heigth = FreeImage_GetHeight( bitmapcvt );

            auto numBytes = ( *width ) * ( *heigth ) * ( *bpp ) / 8;

            pixels = new char[ numBytes ];

            memcpy( pixels, FreeImage_GetBits( bitmapcvt ), numBytes );

            FreeImage_Unload( bitmapcvt );
        }
    }

    if( memory )
        FreeImage_CloseMemory( memory );

    return pixels;
}

// *********************************
//
char *                  LoadRAWImageImpl( const std::string & filePath, SizeType * size )
{
    assert( size != nullptr );
    *size = File::Size( filePath );

    auto buffer = new char[ *size ];

    File::Read( buffer, filePath );

    return buffer;
}

// *********************************
//
bool                    SaveBMPImageImpl( const std::string & filePath, const char * data, UInt32 width, UInt32 height, UInt32 bpp )
{
    FIBITMAP * bitmap = FreeImage_Allocate( width, height, bpp );

    auto bits = FreeImage_GetBits( bitmap );

    memcpy( bits, data, width * height * bpp / 8 );

    auto res = FreeImage_Save( FREE_IMAGE_FORMAT::FIF_BMP, bitmap, filePath.c_str(), BMP_DEFAULT ) ? true : false;

    FreeImage_Unload( bitmap );

    return res;
}

// *********************************
//
void                    SaveRAWImageImpl( const std::string & filePath, const char * data, SizeType size )
{
    auto f = File::Open( filePath, File::FOMReadWrite );
    f.Write( data, size );
    f.Close();
}

// *********************************
//
char *                    ResizeImpl        ( const char * in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 newWidth, UInt32 newHeight, FilterType ft )
{
    FIBITMAP * inBitmap = nullptr;
    if( bpp <= 32 )
    {
        inBitmap = FreeImage_Allocate( ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }
    else
    {
        inBitmap = FreeImage_AllocateT( FIT_RGBAF, ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }

    memcpy( FreeImage_GetBits( inBitmap ), in, width * height * bpp / 8 );

    auto b = FreeImage_GetBPP( inBitmap );
    
    auto outBitmap = FreeImage_Rescale( inBitmap, ( int )newWidth, ( int )newHeight, ToFIFilter( ft ) );

    auto ob = FreeImage_GetBPP( inBitmap );

    // FIXME: what are b and ob required for?
    {b; ob; }
    //FreeImage_AdjustColors( outBitmap, 1.0, 1.0, 1.0, 1 );
    //outBitmap = FreeImage_ConvertTo32Bits( outBitmap );

    auto numBytes = newWidth * newHeight * bpp / 8;

    char * pixels = new char[ numBytes ];
    memcpy( pixels, FreeImage_GetBits( outBitmap ), numBytes );

    FreeImage_Unload( inBitmap );
    FreeImage_Unload( outBitmap );

    return pixels;
}

// *********************************
//
char *                  MakeThumbnailImpl   ( const char * in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 maxSize )
{
    FIBITMAP * inBitmap = nullptr;
    if( bpp <= 32 )
    {
        inBitmap = FreeImage_Allocate( ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }
    else
    {
        inBitmap = FreeImage_AllocateT( FIT_RGBAF, ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }
    
    memcpy( FreeImage_GetBits( inBitmap ), in, width * height * bpp / 8 );
    
    auto b = FreeImage_GetBPP( inBitmap );

    if( b > 32 ) 
    {
        FIBITMAP * inBitmapConverted = FreeImage_ConvertTo32Bits( inBitmap );
        FreeImage_Unload( inBitmap );
        inBitmap = inBitmapConverted;
    }

    
    auto outBitmap = FreeImage_MakeThumbnail( inBitmap, ( int )maxSize, true );
    
    auto outW = FreeImage_GetWidth( outBitmap );
    auto outH = FreeImage_GetHeight( outBitmap );

    if( outW != maxSize || outH != maxSize )
    {
        FIBITMAP * cropedBitmap = nullptr;
        
        cropedBitmap = FreeImage_Allocate( ( int )maxSize, ( int )maxSize, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );

        auto copiedBitmap = FreeImage_Copy( outBitmap, 0, 0, outW, outH );

        FreeImage_Paste( cropedBitmap, outBitmap, ( maxSize - outW ) / 2, ( maxSize - outH ) / 2, 255 );

        FreeImage_Unload( outBitmap );
        FreeImage_Unload( copiedBitmap );

        outBitmap = cropedBitmap;
    }
    
    auto numBytes = maxSize * maxSize * bpp / 8;
    
    char * pixels = new char[ numBytes ];
    memcpy( pixels, FreeImage_GetBits( outBitmap ), numBytes );
    
    FreeImage_Unload( inBitmap );
    FreeImage_Unload( outBitmap );
    
    return pixels;
}

char *                    BlurImageImpl     ( const char * data, UInt32 width, UInt32 height, UInt32 bpp, Float32 blurSize, BlurType filter ) 
{
    switch( filter ) 
    {
        case BlurType::BT_BOX:
            return BoxBlurImage( data, width, height, bpp, blurSize );
        case BlurType::BT_TRIANGLE:
        {
            char * pass1out = BoxBlurImage( data, width, height, bpp, blurSize / 2.f );
            char * pass2out = BoxBlurImage( pass1out, width, height, bpp, blurSize / 2.f );
            delete [] pass1out;
            return pass2out;
        }
        case BlurType::BT_GAUSSIAN:
        {
            char * pass1out = BoxBlurImage( data, width, height, bpp, blurSize / 3.f );
            char * pass2out = BoxBlurImage( pass1out, width, height, bpp, blurSize / 3.f );
            char * pass3out = BoxBlurImage( pass2out, width, height, bpp, blurSize / 3.f );
            delete [] pass1out;
            delete [] pass2out;
            return pass3out;
        }
        default:
            return nullptr;
    }
}

// *********************************
//
char *                    BoxBlurImage      ( const char * data, UInt32 width, UInt32 height, UInt32 bpp, Float32 blurSize )
{
    //blurSize = 0.1f;

    //{ data; width; height; }

    //const char test_data [] = { 
    //    0,0,0,0, 0,0,0,0,           0,0,0,0,
    //    0,0,0,0, -1,-1,-1,-1,       0,0,0,0,
    //    0,0,0,0, 0,0,0,0,           0,0,0,0
    //};

    //width = 3;
    //height = 3;

    //SaveBMPImageImpl( "orig.bmp", data, width, height, bpp );
    auto numBytes = width * height * bpp / 8;

    char * tmp = new char[ numBytes ];
    char * out = new char[ numBytes ];

    float kernelSizeDiv = 1.f / ( blurSize * 2 + 1 );

    Int32 blurSizeCeil = ( Int32 ) std::ceilf( blurSize );
    Float32 blurOverlap = 1.f - (blurSizeCeil - blurSize);

    for ( unsigned int y = 0; y < height; ++y )
    {
        Float4 currVal( 0.f, 0.f, 0.f, 0.f );

        for( int i = -blurSizeCeil; i <= blurSizeCeil; ++i )
        {            
            Float32 weight = ( i == - blurSizeCeil || i == blurSizeCeil ) ? blurOverlap : 1.f;

            currVal = currVal + GetPixelColorFloat4( i, y, data, width, height ) * weight;
            currVal = currVal * kernelSizeDiv;
        }

        SetPixelColorFloat4( y, 0, tmp, height, width, currVal );

        for ( unsigned int x = 1; x < width; ++x )
        {
            currVal = currVal + ( 
                                    ( 
                                        GetPixelColorFloat4( x + blurSizeCeil, y, data, width, height ) * blurOverlap +
                                        GetPixelColorFloat4( x + blurSizeCeil - 1, y, data, width, height ) * ( 1.f - blurOverlap )
                                    ) -
                                    (
                                        GetPixelColorFloat4( x - blurSizeCeil - 1, y, data, width, height ) * blurOverlap +
                                        GetPixelColorFloat4( x - blurSizeCeil, y, data, width, height ) * ( 1.f - blurOverlap )
                                    )
                                ) *
                                kernelSizeDiv;
            
            SetPixelColorFloat4( y, x, tmp, height, width, currVal );
        }
    }

    //SaveBMPImageImpl( "blur_v.bmp", tmp, height, width, bpp );

    for ( unsigned int y = 0; y < width; ++y )
    {
        Float4 currVal( 0.f, 0.f, 0.f, 0.f );

        for( int i = -blurSizeCeil; i <= blurSizeCeil; ++i )
        {
            Float32 weight = ( i == - blurSizeCeil || i == blurSizeCeil ) ? blurOverlap : 1.f;

            currVal = currVal + GetPixelColorFloat4( i, y, tmp, height, width ) * weight;
            currVal = currVal * kernelSizeDiv;
        }

        SetPixelColorFloat4( y, 0, out, width, height, currVal );

        for ( unsigned int x = 1; x < height; ++x )
        {            
            currVal = currVal + (
                                    ( 
                                        GetPixelColorFloat4( x + blurSizeCeil, y, tmp, height, width )  * blurOverlap +
                                        GetPixelColorFloat4( x + blurSizeCeil - 1, y, tmp, height, width )  * ( 1.f - blurOverlap )
                                    ) -
                                    (
                                        GetPixelColorFloat4( x - blurSizeCeil - 1, y, tmp, height, width )  * blurOverlap +
                                        GetPixelColorFloat4( x - blurSizeCeil, y, tmp, height, width )  * ( 1.f - blurOverlap )
                                    )
                                ) *
                                kernelSizeDiv;
            
            SetPixelColorFloat4( y, x, out, width, height, currVal );
        }
    }

    delete [] tmp;

    //SaveBMPImageImpl( "blur_h.bmp", out, width, height, bpp );

    return out;
}

// ***********************
//
char *              FlipHorizontalImpl  ( const char * data, UInt32 width, UInt32 height, UInt32 bpp )
{
    FIBITMAP * copyInBitmap = nullptr;
    if( bpp <= 32 )
    {
        copyInBitmap = FreeImage_Allocate( ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }
    else
    {
        copyInBitmap = FreeImage_AllocateT( FIT_RGBAF, ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }

    auto numBytes = width * height * bpp / 8;
    memcpy( FreeImage_GetBits( copyInBitmap ), data, numBytes );

    FreeImage_FlipHorizontal( copyInBitmap );

    char * pixels = new char[ numBytes ];
    memcpy( pixels, FreeImage_GetBits( copyInBitmap ), numBytes );

    FreeImage_Unload( copyInBitmap );
    return pixels;
}

// ***********************
//
char *                  FlipVerticalImpl    ( const char * data, UInt32 width, UInt32 height, UInt32 bpp )
{
    FIBITMAP * copyInBitmap = nullptr;
    if( bpp <= 32 )
    {
        copyInBitmap = FreeImage_Allocate( ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }
    else
    {
        copyInBitmap = FreeImage_AllocateT( FIT_RGBAF, ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }

    auto numBytes = width * height * bpp / 8;
    memcpy( FreeImage_GetBits( copyInBitmap ), data, numBytes );

    FreeImage_FlipVertical( copyInBitmap );

    char * pixels = new char[ numBytes ];
    memcpy( pixels, FreeImage_GetBits( copyInBitmap ), numBytes );

    FreeImage_Unload( copyInBitmap );
    return pixels;
}

// *********************************
//
MemoryChunkConstPtr LoadImage( const std::string & filePath, UInt32 * width, UInt32 * height, UInt32 * bpp, UInt32 * channelNum, bool loadFromMemory )
{    
    auto pixels = LoadImageImpl( filePath, width, height, bpp, channelNum, loadFromMemory );
    auto numBytes = ( *width ) * ( *height ) * ( *bpp ) / 8;

    return pixels ? MemoryChunk::Create( pixels, numBytes ) : nullptr;
}

// *********************************
//
MemoryChunkConstPtr LoadRAWImage( const std::string & filePath )
{
    SizeType size = 0;
    auto buffer = LoadRAWImageImpl( filePath, &size );

    return buffer ? MemoryChunk::Create( buffer, size ) : nullptr;
}

// *********************************
//
bool SaveBMPImage( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp )
{
    return SaveBMPImageImpl( filePath, data->Get(), width, height, bpp );
}

// *********************************
//
void SaveRAWImage( const std::string & filePath, MemoryChunkConstPtr data )
{
    SaveRAWImageImpl( filePath, data->Get(), data->Size() );
}

// *********************************
//
MemoryChunkConstPtr BlurImage( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, Float32 blurSize, BlurType type )
{
    auto numBytes = width * height * bpp / 8;
    auto pixels = BlurImageImpl( data->Get(), width, height, bpp, blurSize, type );

    return MemoryChunk::Create( pixels, numBytes );
}

// ******************************
//
MemoryChunkConstPtr        Resize( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 newWidth, UInt32 newHeight, FilterType ft )
{
    auto numBytes = newWidth * newHeight * bpp / 8;
    auto pixels = ResizeImpl( in->Get(), width, height, bpp, newWidth, newHeight, ft );

    return MemoryChunk::Create( pixels, numBytes );
}

// ***********************
//
MemoryChunkConstPtr     MakeThumbnai    ( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp, UInt32 maxSize )
{
    auto numBytes = maxSize * maxSize * bpp / 8;
    auto pixels = MakeThumbnailImpl( in->Get(), width, height, bpp, maxSize );

    return MemoryChunk::Create( pixels, numBytes );
}

// ***********************
//
MemoryChunkConstPtr        FlipHorizontal  ( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp )
{
    auto pixels = FlipHorizontalImpl( data->Get(), width, height, bpp );

    return MemoryChunk::Create( pixels, data->Size() );
}

// ***********************
//
MemoryChunkConstPtr        FlipVertical    ( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp )
{
    auto pixels = FlipVerticalImpl( data->Get(), width, height, bpp );

    return MemoryChunk::Create( pixels, data->Size() );
}

// ******************************
//
MemoryChunkConstPtr     SaveTGAToHandle ( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 bpp )
{
    FIBITMAP * inBitmap = nullptr;
    if( bpp <= 32 )
    {
        inBitmap = FreeImage_Allocate( ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }
    else
    {
        inBitmap = FreeImage_AllocateT( FIT_RGBAF, ( int )width, ( int )height, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK );
    }

    memcpy( FreeImage_GetBits( inBitmap ), in->Get(), width * height * bpp / 8 );

    FIMEMORY * destStream = FreeImage_OpenMemory();  

    MemoryChunkConstPtr ret = nullptr;

    if( FreeImage_SaveToMemory( FIF_TARGA, inBitmap, destStream, TARGA_SAVE_RLE ) )
    {
        FreeImage_SeekMemory( destStream, 0, SEEK_END );
        auto size = FreeImage_TellMemory( destStream );
        
        FreeImage_SeekMemory( destStream, 0, SEEK_SET );
        
        BYTE * pixelsR = nullptr;
        DWORD readBytes = 0;
        FreeImage_AcquireMemory( destStream, &pixelsR, &readBytes );

        char * pixels = new char[ size ];

        memcpy( pixels, pixelsR, size );

        FreeImage_Unload( inBitmap );
        FreeImage_CloseMemory( destStream );

        ret = MemoryChunk::Create( ( char * )pixels, size );
    }

    return ret;
}

// ******************************
//
MemoryChunkConstPtr     SwapChannels    ( const MemoryChunkConstPtr & in, UInt32 bpp, UInt32 b, UInt32 g, UInt32 r, UInt32 a )
{
    assert( bpp == 32 );
    { bpp; }

    auto out = MemoryChunk::Create( new char[ in->Size() ], in->Size() );

    auto o = out->GetWritable();

    for( auto p = in->Get(); p < in->Get() + in->Size(); p += 4, o += 4 )
    {
        *( ( UInt32 * )( o ) ) = ( ( UInt32 )p[ 0 ] & b )
                                + ( ( ( UInt32 )p[ 0 ] << 8 ) & b ) 
                                + ( ( ( UInt32 )p[ 0 ] << 16 ) & b )
                                + ( ( ( UInt32 )p[ 0 ] << 24 ) & b )
                                + ( ( UInt32 )p[ 1 ] & g )
                                + ( ( ( UInt32 )p[ 1 ] << 8 ) & g ) 
                                + ( ( ( UInt32 )p[ 1 ] << 16 ) & g )
                                + ( ( ( UInt32 )p[ 1 ] << 24 ) & g )
                                + ( ( UInt32 )p[ 2 ] & r )
                                + ( ( ( UInt32 )p[ 2 ] << 8 ) & r ) 
                                + ( ( ( UInt32 )p[ 2 ] << 16 ) & r )
                                + ( ( ( UInt32 )p[ 2 ] << 24 ) & r )
                                + ( ( UInt32 )p[ 3 ] & a )
                                + ( ( ( UInt32 )p[ 3 ] << 8 ) & a ) 
                                + ( ( ( UInt32 )p[ 3 ] << 16 ) & a )
                                + ( ( ( UInt32 )p[ 3 ] << 24 ) & a )
                                ;
        auto r1 = *( ( UInt32 * )( o ) );
        { r1; }
    }

    return out;
}

MemoryChunkConstPtr     AddImages       ( const MemoryChunkConstPtr & in1, const MemoryChunkConstPtr & in2 )
{
    assert( in1->Size() == in2->Size() );

    auto out = MemoryChunk::Create( new char[ in1->Size() ], in1->Size() );

    auto o = out->GetWritable();

    for( auto p1 = in1->Get(), p2 = in2->Get(); p1 < in1->Get() + in1->Size(); p1 += 4, p2 += 4, o += 4 ) 
    {
        *( (UInt32 * )( o ) ) = *((UInt32 * )(&p1[ 0 ])) + *((UInt32 * )(&p2[ 0 ]));
    }

    return out;
}

} // image
} // bv
