#include "TextureHelpers.h"

#include <memory>
#include <sstream>

#include "System/FileIO.h"
#include "FreeImagePlus.h"

#include "Engine/Models/Resources/ModelTextureManager.h"

namespace bv { namespace model {

// *********************************
//
MemoryChunkConstPtr TextureHelper::LoadImg( const std::string & filePath, int * width, int * heigth, int * bpp, bool loadFromMemory )
{
    auto fipImg = std::make_shared< fipImage >();

    if( loadFromMemory )
    {
        // FIXME: Crash when file doesn't exist
        char * bufToRead = new char[ File::Size( filePath ) ]; 

        int bytes = File::Read( bufToRead, filePath );

        fipMemoryIO fipIO( ( BYTE * ) bufToRead, ( DWORD ) bytes );

        FREE_IMAGE_FORMAT type = fipIO.getFileType();

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
void TextureHelper::WriteBMP( const std::string& filePath, MemoryChunkConstPtr data, int width, int height, int bpp )
{
    fipImage*  fipImg = new fipImage( FREE_IMAGE_TYPE::FIT_BITMAP, width, height, bpp );

    auto pixels = fipImg->accessPixels();

    memcpy( pixels, data->Get(), width * height * bpp / 8 );

    fipImg->save( filePath.c_str() );
}


// *********************************
//
inline unsigned char GetPixelColor( int x, int y, const char* data, int width, int height )
{
    if( x < 0 || x >= width || y < 0 || y >= height )
        return 0;
    return data[ 4 * ( x + y * width ) ];
}

// *********************************
//
inline void SetPixelColor( int x, int y, char* data, int width, int height, char color )
{
    memset( &( data[ 4 * ( x + y * width ) ] ), color, 4 );
}

// *********************************
//
MemoryChunkConstPtr TextureHelper::Blur( MemoryChunkConstPtr data, int width, int height, int bpp, int blurSize )
{
    unsigned int numBytes = width * height * bpp / 8;

    char * tmp = new char[ numBytes ];
    char * out = new char[ numBytes ];

    float kernelSize = float( blurSize * 2 + 1 );

    for ( int y = 0; y < height; ++y )
    {
        for ( int x = 0; x < width; ++x )
        {
            float currVal = 0.f;
            for( int i = -blurSize; i <= blurSize; ++i )
                currVal += GetPixelColor( x + i, y, data->Get(), width, height );
            currVal /= kernelSize;
            if (currVal > 0.f)
                int t = 0;
            SetPixelColor( x, y, tmp, width, height, (unsigned char)(currVal) );
        }
    }

    for ( int x = 0; x < width; ++x )
    {
        for ( int y = 0; y < height; ++y )
        {
            float currVal = 0.f;
            for( int i = -blurSize; i <= blurSize; ++i )
                currVal += GetPixelColor( x, y + i , tmp, height, width );
            currVal /= kernelSize;
            SetPixelColor( x, y, out, width, height, (unsigned char)(currVal) );
        }
    }


    delete [] tmp;
    return std::make_shared< MemoryChunk >( out, numBytes );
}

} // model
} // bv