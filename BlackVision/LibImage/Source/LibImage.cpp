#include "LibImage.h"
#include "IO/FileIO.h"

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

	Float4 operator * ( Float32 f )
	{
		return Float4( x * f, y * f, z * f, w * f );
	}
};

// *********************************
//
ImageProperties GetImageProps( const std::string & imageFilePath )
{
	if( !File::Exists( imageFilePath ) )
	{
		ImageProperties iprops;
		iprops.error = std::string("File doesn't exist");
		return iprops;
	}

	auto size = File::Size( imageFilePath );

	FREE_IMAGE_FORMAT fiff = FreeImage_GetFileType( imageFilePath.c_str(), (int) size );

	if( fiff == FIF_UNKNOWN )
	{
		ImageProperties iprops;
		iprops.error = std::string("Unknown file format");
		return iprops;
	}

	FIBITMAP * bitmap = FreeImage_Load( fiff, imageFilePath.c_str(), BMP_DEFAULT );

	if( bitmap == nullptr )
	{
		ImageProperties iprops;
		iprops.error = std::string("Cannot load file: ") + imageFilePath;
		return iprops;
	}

	auto bbp		= FreeImage_GetBPP( bitmap );
	auto colorType	= FreeImage_GetColorType( bitmap );

	ImageFormat format;

	if( colorType == FIC_RGBALPHA && bbp == 32 )
		format = bv::image::ImageFormat::IF_A8R8G8B8;
	else if ( colorType == FIC_RGB && bbp == 24 )
		format = bv::image::ImageFormat::IF_R8G8B8;
	else if ( bbp == 8 )
		format = bv::image::ImageFormat::IF_A8;
	else
	{
		ImageProperties iprops;
		iprops.error = std::string("Image format not supported");
		return iprops;
	}

	ImageProperties iprops;

	iprops.format = format;
	iprops.width = ( bv::SizeType )FreeImage_GetWidth( bitmap );
	iprops.height = ( bv::SizeType )FreeImage_GetHeight( bitmap );

	return iprops;
}

namespace {

// *********************************
//
FIBITMAP * ConvertToNearestSupported( FIBITMAP * bitmap )
{
	auto bpp = FreeImage_GetBPP( bitmap );

	auto imgType = FreeImage_GetImageType( bitmap );

	if( imgType == FIT_BITMAP )
	{
		switch( bpp )
		{
		case 32:
			return bitmap;
		case 24:
		case 16:
			return FreeImage_ConvertTo32Bits( bitmap );
		case 8:
		case 4:
		case 1:
			return FreeImage_ConvertTo8Bits( bitmap );
		}
	}
	else if( imgType < FIT_DOUBLE )
	{
		return FreeImage_ConvertToFloat( bitmap );
	}
	else if( imgType >= FIT_RGB16 && imgType <= FIT_RGBAF )
	{
		return FreeImage_ConvertToRGBF( bitmap );
	}

	assert( !"Not supported texture format" );
	return nullptr;
}

} // anonymous

// *********************************
//
MemoryChunkConstPtr LoadImage( const std::string & filePath, UInt32 * width, UInt32 * heigth, UInt32 * bpp, bool loadFromMemory )
{	
	FIBITMAP * bitmap = nullptr;

	if( !File::Exists( filePath ) )
		return nullptr;

	auto size = File::Size( filePath );

	FREE_IMAGE_FORMAT fiff = FreeImage_GetFileType( filePath.c_str(), (int) size );

    if( loadFromMemory )
    {
        char * bufToRead = new char[ size ]; 

        SizeType bytes = File::Read( bufToRead, filePath );

		FIMEMORY * memory = FreeImage_OpenMemory( ( BYTE * ) bufToRead, ( DWORD ) bytes );

		bitmap = FreeImage_LoadFromMemory( fiff, memory, BMP_DEFAULT );

        if( bitmap == nullptr )
        {
            return nullptr;
        }

        delete[] bufToRead;
    }
    else
    {
		bitmap = FreeImage_Load( fiff, filePath.c_str(), BMP_DEFAULT );
        if( bitmap == nullptr )
        {
            return nullptr;
        }
    }

	bitmap = ConvertToNearestSupported( bitmap );

	//bitmap = FreeImage_ConvertTo32Bits( bitmap );

    if( bitmap == nullptr )
	{
		return nullptr;
	}

	*width  = FreeImage_GetWidth( bitmap );
	*heigth = FreeImage_GetHeight( bitmap );
	*bpp    = FreeImage_GetBPP( bitmap );

    auto numBytes = ( *width ) * ( *heigth ) * ( *bpp ) / 8;

    char * pixels = new char[ numBytes ];

    memcpy( pixels, FreeImage_GetBits( bitmap ), numBytes );

    return std::make_shared< MemoryChunk >( pixels, numBytes );
}

// *********************************
//
MemoryChunkConstPtr LoadRAWImage( const std::string & filePath )
{
	auto size = File::Size( filePath );

	auto buffer = new char[ size ];
	File::Read( buffer, filePath );

	return std::make_shared< MemoryChunk >( buffer, (SizeType)size );
}

// *********************************
//
void SaveRAWImage( const std::string & filePath, MemoryChunkConstPtr data )
{
	auto f = File::Open( filePath, File::FOMReadWrite );
	f.Write( data->Get(), data->Size() );
	f.Close();
}

// *********************************
//
bool SaveBMPImage( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp )
{
	FIBITMAP * bitmap = FreeImage_Allocate( width, height, bpp );

	auto bits = FreeImage_GetBits( bitmap );

    memcpy( bits, data->Get(), width * height * bpp / 8 );

	return FreeImage_Save( FREE_IMAGE_FORMAT::FIF_BMP, bitmap, filePath.c_str(), BMP_DEFAULT ) ? true : false;
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
	x = std::max( 0, std::min( x, ( Int32 )width - 1 ) );
	y = std::max( 0, std::min( y, ( Int32 )height - 1 ) );

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

// *********************************
//
MemoryChunkConstPtr BlurImage( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, unsigned int blurSize )
{
    auto numBytes = width * height * bpp / 8;

    char * tmp = new char[ numBytes ];
    char * out = new char[ numBytes ];

    float kernelSize = float( blurSize * 2 + 1 );

    for ( unsigned int y = 0; y < height; ++y )
    {
        for ( unsigned int x = 0; x < width; ++x )
        {
			Float4 currVal( 0.f, 0.f, 0.f, 0.f );
            for( int i = - (Int32)blurSize; i <= (Int32)blurSize; ++i )
			{
				currVal = currVal + GetPixelColorFloat4( x + i, y, data->Get(), width, height );
			}

            currVal = currVal * ( 1.f / kernelSize );
            
            SetPixelColorFloat4( x, y, tmp, width, height, currVal );
        }
    }

    for ( unsigned int x = 0; x < width; ++x )
    {
        for ( unsigned int y = 0; y < height; ++y )
        {
            Float4 currVal( 0.f, 0.f, 0.f, 0.f );
            for( int i = -(int)blurSize; i <= (int)blurSize; ++i )
			{
				currVal = currVal + GetPixelColorFloat4( x, y + i , tmp, height, width );
			}

            currVal = currVal * ( 1.f / kernelSize );
            
			SetPixelColorFloat4( x, y, out, width, height, currVal );
        }
    }

    delete [] tmp;
    return std::make_shared< MemoryChunk >( out, numBytes );
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

// ******************************
//
MemoryChunkConstPtr		Resize( const MemoryChunkConstPtr & in, UInt32 width, UInt32 height, UInt32 newWidth, UInt32 newHeight, FilterType ft )
{
	auto inBitmap = FreeImage_ConvertFromRawBits( (BYTE*)in->Get(), ( int )width, ( int )height, ( int )width * 4, 32, 255, 255, 255 );

	auto outBitmap = FreeImage_Rescale( inBitmap, ( int )newWidth, ( int )newHeight, ToFIFilter( ft ) );

	//FreeImage_AdjustColors( outBitmap, 1.0, 1.0, 1.0, 1 );

	outBitmap = FreeImage_ConvertTo32Bits( outBitmap );

	auto numBytes = newWidth * newHeight * 4;

    char * pixels = new char[ numBytes ]; // FIXME: Use normal allocation to free it with free not delete []
    memcpy( pixels, FreeImage_GetBits( outBitmap ), numBytes );

	return MemoryChunk::Create( pixels, numBytes );
}

} // image
} // bv