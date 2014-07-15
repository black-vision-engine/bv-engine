#include "Texture2DImpl.h"

#include <cstring>
#include <cassert>

namespace bv {

// *********************************
//
Texture2DImpl::Texture2DImpl  ( TextureFormat format, unsigned int width, unsigned int height, DataBuffer::Semantic semantic )
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
size_t          Texture2DImpl::GetDataSize      () const
{
    return m_dataSize;
}

// *********************************
//
MemoryChunkConstPtr Texture2DImpl::GetData      () const
{
    return m_data;
}

// *********************************
//
bool            Texture2DImpl::SetRawData       ( MemoryChunkConstPtr data, TextureFormat format, unsigned int width, unsigned int height )
{
    SetFormat( format );
    SetWidth( width );
    SetHeight( height );

    m_data = data;

    SetChanged( true );

    return true;
}

// *********************************
//
unsigned int    Texture2DImpl::SizeInBytes     ( TextureFormat format, unsigned int width, unsigned int height )
{
    return GetPixelSize( format ) * width * height;
}

} //bv
