#include "TextureHelpers.h"

#include <sstream>
#include "System/FileIO.h"

#include "FreeImagePlus.h"
#include <memory>

namespace bv { namespace model {

// *********************************
//
char* TextureHelper::LoadImg( const std::string& filePath, int* width, int* heigth, int* bpp, bool loadFromMemory )
{
    std::shared_ptr< fipImage > fipImg = std::make_shared< fipImage >();

    if( loadFromMemory )
    {
        char* bufToRead = new char[ File::Size( filePath ) ]; 

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

    return pixels;
}


// *********************************
//
void TextureHelper::WriteBMP( const std::string& filePath, const char* data, int width, int height, int bpp )
{
    fipImage*  fipImg = new fipImage( FREE_IMAGE_TYPE::FIT_BITMAP, width, height, bpp );

    auto pixels = fipImg->accessPixels();

    memcpy( pixels, data, width * height * bpp / 8 );

    fipImg->save( filePath.c_str() );
}


} // model
} // bv