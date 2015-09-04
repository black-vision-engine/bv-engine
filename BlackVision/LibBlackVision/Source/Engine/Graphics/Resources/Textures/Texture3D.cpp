#include "Texture3D.h"

#include <algorithm>

namespace bv {

// *********************************
//
Texture3D::Texture3D                    ( TextureFormat format, UInt32 width, UInt32 height, UInt32 depth, DataBuffer::Semantic semantic )
    : Texture( format, TextureType::T_3D, semantic, 1, 1 )
    , m_width( width )
    , m_height( height )
    , m_depth( depth )
{
	m_data.resize( m_layers * m_levels );
}

// *********************************
//
Texture3D::Texture3D                    ( TextureFormat format, UInt32 width, UInt32 height, UInt32 depth, DataBuffer::Semantic semantic, UInt32 levels )
    : Texture( format, TextureType::T_3D, semantic, 1, levels )
    , m_width( width )
    , m_height( height )
    , m_depth( depth )
{
	m_data.resize( m_layers * m_levels );
}

// *********************************
//
Texture3D::~Texture3D       ()
{
}

// *********************************
//
UInt32    Texture3D::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    Texture3D::GetHeight         ( UInt32 level ) const
{
	return std::max( m_height >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    Texture3D::GetDepth         ( UInt32 level ) const
{
	return std::max( m_depth >> level, ( UInt32 )1 );
}

// *********************************
//
MemoryChunkConstPtr    Texture3D::GetData        ( UInt32 level ) const
{
	return GetDataChunk( 0, level );
}

// *********************************
//
void				    Texture3D::SetData      ( MemoryChunkConstPtr data, UInt32 level ) 
{
	return SetDataChunk( data, 0, level );
}

// *********************************
//
void				    Texture3D::SetData		( const std::vector< MemoryChunkConstPtr > & data )
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
SizeType    Texture3D::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level )  * GetHeight( level )  * GetDepth( level );
}

// *********************************
//
SizeType    Texture3D::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 height, UInt32 depth, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level ) * ( height >> level ) * ( depth >> level );
}

} //bv
