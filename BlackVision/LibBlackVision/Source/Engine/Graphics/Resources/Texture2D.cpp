#include "Texture2D.h"

#include <algorithm>

namespace bv {

// *********************************
//
Texture2D::Texture2D                    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic )
	: Texture( format, TextureType::T_2D, semantic, 1, 1, 1 )
    , m_width( width )
    , m_height( height )
{
}

// *********************************
//
Texture2D::Texture2D                    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 levels )
	: Texture( format, TextureType::T_2D, semantic, 1, levels, levels )
    , m_width( width )
    , m_height( height )
{
}

// *********************************
//
Texture2D::~Texture2D       ()
{
}

// *********************************
//
UInt32    Texture2D::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    Texture2D::GetHeight        ( UInt32 level ) const
{
	return std::max( m_height >> level, ( UInt32 )1 );
}

// *********************************
//
MemoryChunkConstPtr    Texture2D::GetData        ( UInt32 level ) const
{
	return GetDataChunk( 0, level );
}

// *********************************
//
void				    Texture2D::SetData      ( MemoryChunkConstPtr data, UInt32 level ) 
{
	return SetDataChunk( data, 0, level );
}

// *********************************
//
void				    Texture2D::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	m_data.clear();

	for( auto & chunk : data )
	{
		m_data.push_back( chunk );
	}

    m_levels = ( UInt32 )m_data.size();
}

// *********************************
//
SizeType    Texture2D::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level ) * GetHeight( level );
}

// *********************************
//
SizeType    Texture2D::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 height, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level ) * ( height >> level );
}

} //bv
