#include "TextureLoader.h"

#include "FreeImagePlus.h"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace bv { namespace model {

TextureLoader::TextureLoader( bool loadFormMemory )
    : m_loadFromMemory( loadFormMemory )
{}

ResourceHandle *        TextureLoader::LoadResource        ( IResource* res )  const
{
    boost::filesystem::path filepath( res->GetFilePath() );
    std::string errMsg( "Cannot read file: " + res->GetFilePath() ); 

    if( !exists( filepath ) )
    {
        throw std::runtime_error( errMsg );
        return nullptr;
    }

    fipImage*  fipImg = new fipImage();

    if( m_loadFromMemory )
    {
        boost::filesystem::ifstream file(filepath);

        uintmax_t size = file_size(filepath);

        char* bufToRead = new char[ ( unsigned int ) size ];
    
        file.read( bufToRead, size );

        fipMemoryIO fipIO( ( BYTE * ) bufToRead, ( DWORD ) size );

        FREE_IMAGE_FORMAT type = fipIO.getFileType();

        if( !fipImg->loadFromMemory( fipIO ) )
        {
            throw std::runtime_error( errMsg );
            return nullptr;
        }

        delete bufToRead;
    }
    else
    {
        if( !fipImg->load( res->GetFilePath().c_str() ) )
        {
            throw std::runtime_error( errMsg );
            return nullptr;
        }
    }

    if(!fipImg->convertTo32Bits())
    {
        throw std::runtime_error( "Cannot convert texture to bitmap" );
    }

    //FIXME: memcpy
    return new ResourceHandle( (char*)fipImg->accessPixels(), fipImg->getWidth() * fipImg->getHeight() * ( fipImg->getBitsPerPixel() / 8 ), fipImg->getWidth(), fipImg->getHeight() );
}

} // model
} // bv