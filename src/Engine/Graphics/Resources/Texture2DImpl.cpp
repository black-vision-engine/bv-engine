#include "Texture2DImpl.h"

#include <cstring>
#include <cassert>

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
}

// *********************************
//
size_t          Texture2DImpl::GetDataSize     () const
{
    return m_dataSize;
}

// *********************************
//
MemoryChunkConstPtr  Texture2DImpl::GetData         ()
{
    return m_data;
}

// *********************************
//
MemoryChunkConstPtr Texture2DImpl::GetData         () const
{
    return m_data;
}

// *********************************
//
bool            Texture2DImpl::WriteBits       ( MemoryChunkConstPtr data, TextureFormat format, int width, int height )
{
    //AllocateMemory( format, width, height );

    SetFormat( format );
    SetWidth( width );
    SetHeight( height );

    m_data = data;
    //memcpy( m_data, data, GetDataSize() );

    SetChanged( true );

    return true;
}

// *********************************
//
bool            Texture2DImpl::AllocateMemory  ( TextureFormat format, int width, int height )
{
    assert( false );
    unsigned int newSize = SizeInBytes( format, width, height );

    bool changed = false;

    if( GetDataSize() != newSize )
    {
        //delete[] m_data;
        //m_data = new char[ newSize ];
    
        changed = true;
    }

    m_dataSize = newSize;

    return changed;
}

// *********************************
//
bool            Texture2DImpl::AllocateMemory  ()
{
    return AllocateMemory( m_format, GetWidth(), GetHeight() );
}

// *********************************
//
unsigned int    Texture2DImpl::SizeInBytes     ( TextureFormat format, int width, int height )
{
    return GetPixelSize( format ) * width * height;
}

} //bv
