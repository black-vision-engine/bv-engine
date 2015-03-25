#include "Texture2DImpl.h"

#include <cstring>
#include <cassert>

namespace bv {

// *********************************
//
Texture2DImpl::Texture2DImpl  ( TextureFormat format, SizeType width, SizeType height, DataBuffer::Semantic semantic )
    : Texture2D( format, width, height, semantic )
	, m_data( 1 )
{
}

// *********************************
//
Texture2DImpl::~Texture2DImpl  ()
{
}

// *********************************
//
UInt32 Texture2DImpl::GetNumLevels			() const
{
	return ( UInt32 ) m_data.size();
}

// *********************************
//
SizeType	Texture2DImpl::GetDataSize      ( UInt32 level ) const
{
	if( level < m_data.size() )
	{
		return m_data[ level ]->Size();
	}
	else
	{
		assert( !"Should never be here" );
		return 0;
	}
}

// *********************************
//
MemoryChunkConstPtr Texture2DImpl::GetData  ( UInt32 level ) const
{
	if( level < m_data.size() )
	{
		return m_data[ level ];
	}
	else
	{
		assert( !"Should never be here" );
		return nullptr;
	}
}

// *********************************
//
bool            Texture2DImpl::SetRawData    ( MemoryChunkConstPtr data, TextureFormat format, SizeType width, SizeType height, UInt32 level )
{
    assert( data->Size() == SizeInBytes( format, width, height ) || data->Size() == 0 );

    SetFormat( format );
    SetWidth( width );
    SetHeight( height );

	if( level < m_data.size() )
	{
		m_data[ level ] = data;
	}
	else
	{
		m_data.resize( level + 1 );
		m_data[ level ] = data;
	}

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
