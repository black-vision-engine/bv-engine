#include "TextureLoader.h"
#include "TextureHelpers.h"

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
    int width   = 0;
    int height  = 0;
    int bpp     = 0;

    char* data = TextureHelper::LoadImg( res->GetFilePath(), &width, &height, &bpp );

    if ( data != nullptr )
    {
        auto texExtra = new TextureExtraData( width, height, bpp, TextureFormat::F_A8R8G8B8, TextureType::T_2D );
        return new ResourceHandle( data, width * height * bpp / 8, texExtra );
    }
    else
        return nullptr;
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