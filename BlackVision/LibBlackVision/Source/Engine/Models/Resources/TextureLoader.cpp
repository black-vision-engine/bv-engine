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
    unsigned int width   = 0;
    unsigned int height  = 0;
    unsigned int bpp     = 0;

	auto filePath = res->GetFilePath();

	MemoryChunkConstPtr data = nullptr;

	if( filePath.find(".raw") != std::string::npos)
		data = TextureHelper::LoadRAW( filePath );
	else
		data = TextureHelper::LoadImg( filePath, &width, &height, &bpp );

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
SizeType TextureExtraData::GetWidth            () const
{
    return m_width;
}

// ******************************
//
SizeType TextureExtraData::GetHeight           () const
{
    return m_height;
}

// ******************************
//
SizeType TextureExtraData::GetBitsPerPixel     () const
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
TextureExtraData::TextureExtraData()
    : ResourceExtraData( ResourceExtraKind::RE_TEXTURE )
{
}

// ******************************
//
TextureExtraData::TextureExtraData( SizeType w, SizeType h, SizeType bitsPerPixel, TextureFormat format, TextureType type )
    : ResourceExtraData( ResourceExtraKind::RE_TEXTURE )
    , m_width( w )
    , m_height( h )
    , m_bitsPerPixel( bitsPerPixel )
    , m_format( format )
    , m_type( type )
{}

} // model
} // bv
