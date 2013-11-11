#include "TextureLoader.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "FreeImagePlus.h"


namespace bv { namespace model {

// ******************************
//
TextureLoader::TextureLoader( bool loadFormMemory )
    : m_loadFromMemory( loadFormMemory )
{}

// ******************************
//
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
        boost::filesystem::ifstream file( filepath, std::ifstream::binary );

        file.seekg (0, file.end);
        auto size = file.tellg();
        file.seekg (0, file.beg);

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

    //FIXME: Add maping freeimage types to bv types
    auto texExtra = new TextureExtraData( fipImg->getWidth(), fipImg->getHeight(), fipImg->getBitsPerPixel(), TextureFormat::F_A8R8G8B8, TextureType::T_2D );

    //FIXME: memcpy
    return new ResourceHandle( (char*)fipImg->accessPixels(), fipImg->getWidth() * fipImg->getHeight() * ( fipImg->getBitsPerPixel() / 8 ), texExtra );
}

// ******************************
//
int TextureExtraData::GetWidth            () const
{
    return m_width;
}

// ******************************
//
int TextureExtraData::GetHeight           () const
{
    return m_height;
}

// ******************************
//
int TextureExtraData::GetBitsPerPixel     () const
{
    return m_bitsPerPixel;
}

// ******************************
//
TextureFormat TextureExtraData::GetFormat () const
{
    return m_format;
}

// ******************************
//
TextureType TextureExtraData::GetType     () const
{
    return m_type;
}


// ******************************
//
TextureExtraData::TextureExtraData(int w, int h, int bitsPerPixel, TextureFormat format, TextureType type)
    : ResourceExtraData( ResourceExtraKind::RE_TEXTURE )
    , m_width( w )
    , m_height( h )
    , m_bitsPerPixel( bitsPerPixel )
    , m_format( format )
    , m_type( type )
{}

} // model
} // bv