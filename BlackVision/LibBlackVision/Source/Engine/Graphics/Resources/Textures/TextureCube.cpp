#include "TextureCube.h"

#include <algorithm>

namespace bv {
	
// *********************************
//
TextureCube::TextureCube                ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic )
	: Texture( format, TextureType::T_CUBE, semantic, 1, 1 )
    , m_width( width )
    , m_height( height )
{
	m_data.resize( CUBEMAP_FACES_NUM * m_levels * m_layers );
}

// *********************************
//
TextureCube::TextureCube                ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 levels )
	: Texture( format, TextureType::T_CUBE, semantic, 1, levels )
    , m_width( width )
    , m_height( height )
{
	m_data.resize( CUBEMAP_FACES_NUM * m_levels * m_layers );
}

// *********************************
//
TextureCube::~TextureCube       ()
{
}

// *********************************
//
UInt32    TextureCube::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    TextureCube::GetHeight         ( UInt32 level ) const
{
	return std::max( m_height >> level, ( UInt32 )1 );
}

// *********************************
//
MemoryChunkConstPtr    TextureCube::GetData        ( UInt32 face, UInt32 level ) const
{
	return GetDataChunk( 0, ( face * m_levels ) + level );
}

// *********************************
//
void				    TextureCube::SetData      ( MemoryChunkConstPtr data, UInt32 face, UInt32 level ) 
{
	return SetDataChunk( data, 0, ( face * m_levels ) + level );
}

// *********************************
//
void				    TextureCube::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	m_data.clear();

	for( auto & chunk : data )
	{
		m_data.push_back( chunk );
	}

    m_levels = ( UInt32 )m_data.size() / CUBEMAP_FACES_NUM;
}

// *********************************
//
SizeType    TextureCube::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level )  * GetHeight( level );
}

// *********************************
//
SizeType    TextureCube::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 height, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level ) * ( height >> level );
}

} //bv
