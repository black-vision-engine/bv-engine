#include "Texture2DImpl.h"

#include <cstring>


namespace bv {

// *********************************
//
Texture2DImpl::Texture2DImpl  ( TextureFormat format, int width, int height, DataBuffer::Semantic semantic )
    : Texture2D( format, width, height, semantic )
    , m_dataSize( 0 )
    , m_data( nullptr )
{
}

// *********************************
//
Texture2DImpl::~Texture2DImpl  ()
{
    delete[] m_data;
}

// *********************************
//
size_t          Texture2DImpl::GetDataSize     () const
{
    return m_dataSize;
}

// *********************************
//
char *          Texture2DImpl::GetData         ()
{
    return m_data;
}

// *********************************
//
const char *    Texture2DImpl::GetData         () const
{
    return m_data;
}

// *********************************
//
bool            Texture2DImpl::WriteBits       ( const char * data, TextureFormat format, int width, int height )
{
    unsigned int newSize = GetPixelSize( format ) * width * height;

    SetFormat( format );
    SetWidth( width );
    SetHeight( height );

    if( GetDataSize() != newSize )
    {
        delete[] m_data;
        m_data = new char[ newSize ];
    }

    memcpy( m_data, data, newSize );
    m_dataSize = newSize;

    SetChanged( true );

    return true;
}

} //bv
