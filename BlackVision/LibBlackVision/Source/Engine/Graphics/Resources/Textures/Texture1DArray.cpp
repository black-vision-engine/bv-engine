#include "Texture1DArray.h"

namespace bv {

// *********************************
//
Texture1DArray::Texture1DArray                    ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels )
	: Texture( format, TextureType::T_1D, semantic, levels, layers * levels )
    , m_width( width )
    , m_layers( layers )
{
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
UInt32    Texture1DArray::GetNumLayers		() const
{
	return m_layers;
}

// *********************************
//
MemoryChunkConstPtr    Texture1DArray::GetData        ( UInt32 layer, UInt32 level ) const
{
	return GetDataChunk( GetIndex( layer, level ) );
}

// *********************************
//
void				    Texture1DArray::SetData      ( MemoryChunkConstPtr data, UInt32 layer, UInt32 level ) 
{
	assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, GetIndex( layer, level ) );
}

// *********************************
//
void				    Texture1DArray::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	assert( ( UInt32 )data.size() == m_layers * m_levels );

	m_data.clear();

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

// *********************************
//
UInt32		Texture1DArray::GetIndex			( UInt32 layer, UInt32 level ) const
{
	return layer * m_levels + level;
}

} //bv
