#include "Texture2DArray.h"

namespace bv {

// *********************************
//
Texture2DArray::Texture2DArray                    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels )
	: Texture( format, TextureType::T_2D, semantic, levels, layers * levels )
    , m_width( width )
    , m_height( height )
    , m_layers( layers )
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
UInt32    Texture2DArray::GetNumLayers		() const
{
	return m_layers;
}

// *********************************
//
MemoryChunkConstPtr    Texture2DArray::GetData        ( UInt32 layer, UInt32 level ) const
{
	return GetDataChunk( GetIndex( layer, level ) );
}

// *********************************
//
void				    Texture2DArray::SetData      ( MemoryChunkConstPtr data, UInt32 layer, UInt32 level ) 
{
	assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, GetIndex( layer, level ) );
}

// *********************************
//
void				    Texture2DArray::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	assert( ( UInt32 )data.size() == m_layers * m_levels );

	unsigned int i = 0;
	for( unsigned int layer = 0; layer < m_layers; ++layer )
	{
		for( unsigned int lvl = 0; lvl < m_levels; ++lvl )
		{
			assert( data[ i ]->Size() == RawFrameSize( lvl ) );
			m_data.push_back( data[ i ] );
			i++;
		}
	}
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

// *********************************
//
UInt32	    Texture2DArray::GetIndex		( UInt32 layer, UInt32 level ) const
{
    return layer * m_levels + level;
}

} //bv
