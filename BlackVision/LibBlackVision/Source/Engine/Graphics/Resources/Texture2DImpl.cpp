#include "Texture2DImpl.h"

#include <cstring>
#include <cassert>

namespace bv {

// *********************************
//
Texture2DImpl::Texture2DImpl  ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic )
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
UInt32 Texture2DImpl::GetNumLevels		    () const
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
bool            Texture2DImpl::SetRawData    ( const std::vector< MemoryChunkConstPtr > & data, TextureFormat format, UInt32 width, UInt32 height )
{
    SetFormat( format );
    SetWidth( width );
    SetHeight( height );

	m_data.clear();

	for( UInt32 i = 0; i < data.size(); ++i )
	{
		if( i == 0 )
			assert( data[ i ]->Size() == SizeInBytes( format, width >> i, height >> i ) || data[ i ]->Size() == 0 );

		m_data.push_back( data[ i ] );
	}

    SetChanged( true );

    return true;
}

// *********************************
//
SizeType		Texture2DImpl::SizeInBytes     ( TextureFormat format, UInt32 width, UInt32 height )
{
    return GetPixelSize( format ) * width * height;
}

} //bv
