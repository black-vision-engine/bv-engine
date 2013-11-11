#include "Texture.h"

#include <cassert>

#include "FreeImagePlus.h"


namespace bv
{

//FIXME: implement all functions that Texture 2D should have

int Texture::m_sPixelSize[ TextureFormat::F_TOTAL ] =
{
    4,   // F_A8R8G8B8
    1,   // F_A8
    1,   // F_L8
    4,   //F_R32F
};

// *********************************
//  
Texture::Texture                                ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic )
    : m_format( format )
    , m_type( type )
    , m_semantic( semantic )
    , m_data( nullptr )
    , m_dataSize( 0 )
{
}

// *********************************
//
Texture::~Texture	                            ()
{
}

// *********************************
//
TextureFormat        Texture::GetFormat	    () const
{
    return m_format;
}

// *********************************
//
TextureType          Texture::GetType        () const
{
    return m_type;
}

// *********************************
//
DataBuffer::Semantic    Texture::GetSemantic    () const
{
    return m_semantic;
}

// *********************************
//
int                     Texture::GetPixelSize   () const
{
    return GetPixelSize( GetFormat() );
}

// *********************************
//
int                     Texture::GetPixelSize   ( TextureFormat format )
{
    return m_sPixelSize[ static_cast< int >( format ) ];
}

// *********************************
//
size_t                   Texture::GetDataSize     () const
{
    return m_dataSize;
}

// *********************************
//
char *		            Texture::GetData        ()
{
    return m_data;
}

// *********************************
//
const char *            Texture::GetData	    () const
{
    return m_data;
}

// *********************************
//
bool                    Texture::WriteToBuffer   ( const char * memPtr, size_t dataSize )
{
    if ( GetDataSize() != dataSize )
    {
        delete[] m_data;
        m_data = new char[ dataSize ];
        memcpy( m_data, memPtr, dataSize );
        m_dataSize = dataSize;
    }

    return true;
}

// *********************************
//
bool                    Texture::HasSequence     () const
{
    //FIXME: this sucks as hell
    return false;
}

}
