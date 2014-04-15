#include "TextureLoader.h"

#include <sstream>
#include "System/FileIO.h"

#include "FreeImagePlus.h"


namespace bv { namespace model {

// ******************************
//
TextureLoader::TextureLoader( bool loadFormMemory )
    : m_loadFromMemory( loadFormMemory )
{}

// ******************************
//
ResourceHandle *        TextureLoader::LoadResource        ( IResource * res )  const
{
    std::string errMsg( "Cannot read file: " + res->GetFilePath() ); 

    if( !File::Exists( res->GetFilePath() ) )
    {
        throw std::runtime_error( errMsg );
        return nullptr;
    }

    //FIXME: gdzie to jest usuwane? Nigidzie kurwa - i jeszcze rzucanie wyjatkami do chuja wafla
    fipImage * fipImg = new fipImage();

    if( m_loadFromMemory )
    {
        char* bufToRead = new char[ File::Size( res->GetFilePath() ) ]; 

        int bytes = File::Read( bufToRead, res->GetFilePath() );

        fipMemoryIO fipIO( ( BYTE * ) bufToRead, ( DWORD ) bytes );

        FREE_IMAGE_FORMAT type = fipIO.getFileType();

        if( !fipImg->loadFromMemory( fipIO ) )
        {
            throw std::runtime_error( errMsg );
            return nullptr;
        }

        delete[] bufToRead;
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

    //FIXME: Add mapping of freeimage types to bv types
    auto texExtra = new TextureExtraData( fipImg->getWidth(), fipImg->getHeight(), fipImg->getBitsPerPixel(), TextureFormat::F_A8R8G8B8, TextureType::T_2D );

    auto numBytes = fipImg->getWidth() * fipImg->getHeight() * ( fipImg->getBitsPerPixel() / 8 );

    char * pixels = new char[ numBytes ];
    memcpy( pixels, fipImg->accessPixels(), numBytes );

    auto retVal = new ResourceHandle( pixels, numBytes, texExtra );

    delete fipImg;

    return retVal;
}

// ******************************
//
unsigned int TextureExtraData::GetWidth            () const
{
    return m_width;
}

// ******************************
//
unsigned int TextureExtraData::GetHeight           () const
{
    return m_height;
}

// ******************************
//
unsigned int TextureExtraData::GetBitsPerPixel     () const
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
TextureExtraData::TextureExtraData( unsigned int w, unsigned int h, unsigned int bitsPerPixel, TextureFormat format, TextureType type )
    : ResourceExtraData( ResourceExtraKind::RE_TEXTURE )
    , m_width( w )
    , m_height( h )
    , m_bitsPerPixel( bitsPerPixel )
    , m_format( format )
    , m_type( type )
{}

} // model
} // bv