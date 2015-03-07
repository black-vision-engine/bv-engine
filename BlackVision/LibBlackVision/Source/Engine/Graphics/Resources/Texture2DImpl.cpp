#include "Texture2DImpl.h"

#include <cstring>
#include <cassert>

namespace bv {

// *********************************
//
Texture2DImpl::Texture2DImpl  ( TextureFormat format, SizeType width, SizeType height, DataBuffer::Semantic semantic )
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
SizeType	Texture2DImpl::GetDataSize      () const
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
bool            Texture2DImpl::SetRawData       ( MemoryChunkConstPtr data, TextureFormat format, SizeType width, SizeType height )
{
    assert( data->Size() == SizeInBytes( format, width, height ) || data->Size() == 0 );

    SetFormat( format );
    SetWidth( width );
    SetHeight( height );

    m_data = data;
    SetChanged( true );

    return true;
}

// *********************************
//
SizeType		Texture2DImpl::SizeInBytes     ( TextureFormat format, SizeType width, SizeType height )
{
    return GetPixelSize( format ) * width * height;
}

} //bv
