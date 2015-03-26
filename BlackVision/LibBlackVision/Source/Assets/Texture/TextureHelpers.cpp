#include "TextureHelpers.h"

#include <memory>

#include "IO/FileIO.h"
#include "FreeImagePlus.h"


namespace bv {

// *********************************
//
MemoryChunkConstPtr TextureHelper::LoadImg( const std::string & filePath, unsigned int * width, unsigned int * heigth, unsigned int * bpp, bool loadFromMemory )
{
    auto fipImg = std::make_shared< fipImage >();

    if( loadFromMemory )
    {
        // FIXME: Crash when file doesn't exist
        char * bufToRead = new char[ File::Size( filePath ) ]; 

        SizeType bytes = File::Read( bufToRead, filePath );

        fipMemoryIO fipIO( ( BYTE * ) bufToRead, ( DWORD ) bytes );

        FREE_IMAGE_FORMAT type = fipIO.getFileType();
        { type; } // FIXME: suppress unused warning

        if( !fipImg->loadFromMemory( fipIO ) )
        {
            return nullptr;
        }

        delete[] bufToRead;
    }
    else
    {
        if( !fipImg->load( filePath.c_str() ) )
        {
            return nullptr;
        }
    }

    if(!fipImg->convertTo32Bits())
        return nullptr;

    *width  = fipImg->getWidth();
    *heigth = fipImg->getHeight();
    *bpp    = fipImg->getBitsPerPixel();

    auto numBytes = ( *width ) * ( *heigth ) * ( *bpp ) / 8;

    char * pixels = new char[ numBytes ];
    memcpy( pixels, fipImg->accessPixels(), numBytes );

    return std::make_shared< MemoryChunk >( pixels, numBytes );
}

// *********************************
//
MemoryChunkConstPtr TextureHelper::LoadRAW ( const std::string & filePath )
{
	auto size = File::Size( filePath );

	auto buffer = new char[ size ];
	File::Read( buffer, filePath );

	return std::make_shared< MemoryChunk >( buffer, (SizeType)size );
}

// *********************************
//
void TextureHelper::WriteRAW( const std::string & filePath, MemoryChunkConstPtr data )
{
	auto f = File::Open( filePath, File::FOMReadWrite );
	f.Write( data->Get(), data->Size() );
	f.Close();
}

// *********************************
//
void TextureHelper::WriteBMP( const std::string & filePath, MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp )
{
    fipImage*  fipImg = new fipImage( FREE_IMAGE_TYPE::FIT_BITMAP, width, height, bpp );

    auto pixels = fipImg->accessPixels();

    memcpy( pixels, data->Get(), width * height * bpp / 8 );


    fipImg->save( filePath.c_str());
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
MemoryChunkConstPtr TextureHelper::Blur( MemoryChunkConstPtr data, UInt32 width, UInt32 height, UInt32 bpp, unsigned int blurSize )
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

} // bv