#include "Texture1DArray.h"

#include <algorithm>

namespace bv {

// *********************************
//
Texture1DArray::Texture1DArray                    ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels )
	: Texture( format, TextureType::T_1D, semantic, layers, levels )
    , m_width( width )
{
	m_data.resize( m_levels * m_layers );
}

// *********************************
//
Texture1DArray::~Texture1DArray       ()
{
}

// *********************************
//
UInt32    Texture1DArray::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}

// *********************************
//
MemoryChunkConstPtr    Texture1DArray::GetData        ( UInt32 layer, UInt32 level ) const
{
	return GetDataChunk( layer, level );
}

// *********************************
//
void				    Texture1DArray::SetData      ( MemoryChunkConstPtr data, UInt32 layer, UInt32 level ) 
{
	return SetDataChunk( data, layer, level );
}

// *********************************
//
void				    Texture1DArray::SetData		( const std::vector< MemoryChunkConstPtr > & data, UInt32 layers, UInt32 levels )
{
	m_data.clear();

	for( auto & chunk : data )
	{
		m_data.push_back( chunk );
	}

    m_levels = levels;
    m_layers = layers;
}

// *********************************
//
SizeType    Texture1DArray::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level );
}

// *********************************
//
SizeType    Texture1DArray::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level );
}

} //bv
