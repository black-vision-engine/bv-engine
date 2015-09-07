#include "Texture2DArray.h"

#include <algorithm>

namespace bv {

// *********************************
//
Texture2DArray::Texture2DArray                    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels )
	: Texture( format, TextureType::T_2D, semantic, layers, levels, layers * levels )
    , m_width( width )
    , m_height( height )
{
}

// *********************************
//
Texture2DArray::~Texture2DArray       ()
{
}

// *********************************
//
UInt32    Texture2DArray::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    Texture2DArray::GetHeight        ( UInt32 level ) const
{
	return std::max( m_height >> level, ( UInt32 )1 );
}

// *********************************
//
MemoryChunkConstPtr    Texture2DArray::GetData        ( UInt32 layer, UInt32 level ) const
{
	return GetDataChunk( layer, level );
}

// *********************************
//
void				    Texture2DArray::SetData      ( MemoryChunkConstPtr data, UInt32 layer, UInt32 level ) 
{
	return SetDataChunk( data, layer, level );
}

// *********************************
//
void				    Texture2DArray::SetData		( const std::vector< MemoryChunkConstPtr > & data, UInt32 layers, UInt32 levels )
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
SizeType    Texture2DArray::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level ) * GetHeight( level );
}

// *********************************
//
SizeType    Texture2DArray::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 height, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level ) * ( height >> level );
}

} //bv
