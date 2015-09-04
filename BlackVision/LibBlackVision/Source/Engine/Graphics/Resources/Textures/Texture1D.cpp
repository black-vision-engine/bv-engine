#include "Texture1D.h"

#include <algorithm>

namespace bv {

// *********************************
//
Texture1D::Texture1D                    ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic )
    : Texture( format, TextureType::T_1D, semantic, 1, 1 )
    , m_width( width )
{
	m_data.resize( m_levels * m_layers );
}

// *********************************
//
Texture1D::Texture1D                    ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic, UInt32 levels )
    : Texture( format, TextureType::T_1D, semantic, 1, levels )
    , m_width( width )
{
	m_data.resize( m_levels * m_layers );
}


// *********************************
//
Texture1D::~Texture1D       ()
{
}

// *********************************
//
UInt32    Texture1D::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}


// *********************************
//
MemoryChunkConstPtr    Texture1D::GetData        ( UInt32 level ) const
{
	return GetDataChunk( 0, level );
}

// *********************************
//
void				    Texture1D::SetData      ( MemoryChunkConstPtr data, UInt32 level ) 
{
	return SetDataChunk( data, 0, level );
}

// *********************************
//
void				    Texture1D::SetData		( const std::vector< MemoryChunkConstPtr > & data )
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
SizeType    Texture1D::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level );
}

// *********************************
//
SizeType    Texture1D::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level );
}

} //bv
