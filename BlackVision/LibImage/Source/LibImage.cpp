#include "LibImage.h"
#include "IO/FileIO.h"

#include "FreeImage.h"

#include <memory>
#include <cassert>

namespace bv { namespace image
{

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

// *********************************
//
MemoryChunkConstPtr LoadImage( const std::string & filePath, unsigned int * width, unsigned int * heigth, unsigned int * bpp, bool loadFromMemory )
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

	bitmap = FreeImage_ConvertTo32Bits( bitmap );

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
inline void SetPixelColor( Int32 x, Int32 y, char * data, UInt32 width, UInt32 height, char color )
{
    { height; } // FIXME: suppress unused warning

    memset( &( data[ 4 * ( x + y * width ) ] ), color, 4 );
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
            float currVal = 0.f;
            for( int i = - (Int32)blurSize; i <= (Int32)blurSize; ++i )
                currVal += GetPixelColor( x + i, y, data->Get(), width, height );
            currVal /= kernelSize;
            if (currVal > 0.f)
            {
                Int32 t = 0;
                { t; } // FIXME: suppress unused warning
            }
            SetPixelColor( x, y, tmp, width, height, (unsigned char)(currVal) );
        }
    }

    for ( unsigned int x = 0; x < width; ++x )
    {
        for ( unsigned int y = 0; y < height; ++y )
        {
            float currVal = 0.f;
            for( int i = -(int)blurSize; i <= (int)blurSize; ++i )
                currVal += GetPixelColor( x, y + i , tmp, height, width );
            currVal /= kernelSize;
            SetPixelColor( x, y, out, width, height, (unsigned char)(currVal) );
        }
    }

    delete [] tmp;
    return std::make_shared< MemoryChunk >( out, numBytes );
}

} // image
} // bv