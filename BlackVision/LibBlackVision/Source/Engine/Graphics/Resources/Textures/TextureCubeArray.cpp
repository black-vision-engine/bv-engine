#include "TextureCubeArray.h"

namespace bv {

const UInt32	TextureCubeArray::CUBEMAP_FACES_NUM = 6;

// *********************************
//
TextureCubeArray::TextureCubeArray                ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels )
	: Texture( format, TextureType::T_CUBE, semantic, levels, layers * CUBEMAP_FACES_NUM * levels )
    , m_width( width )
    , m_height( height )
	, m_layers( layers )
{
}

// *********************************
//
TextureCubeArray::~TextureCubeArray       ()
{
}

// *********************************
//
UInt32    TextureCubeArray::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    TextureCubeArray::GetHeight         ( UInt32 level ) const
{
	return std::max( m_height >> level, ( UInt32 )1 );
}

// *********************************
//
UInt32    TextureCubeArray::GetNumLayers		() const
{
	return m_layers;
}

// *********************************
//
MemoryChunkConstPtr    TextureCubeArray::GetData        ( UInt32 layer, UInt32 face, UInt32 level ) const
{
	return GetDataChunk( GetIndex( layer, face, level ) );
}

// *********************************
//
void				    TextureCubeArray::SetData      ( MemoryChunkConstPtr data, UInt32 layer, UInt32 face, UInt32 level ) 
{
	assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, GetIndex( layer, face, level ) );
}

// *********************************
//
void				    TextureCubeArray::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	assert( ( UInt32 )data.size() == m_layers * CUBEMAP_FACES_NUM * m_levels );

	m_data.clear();

	unsigned int i = 0;
	for( unsigned int layer = 0; layer < m_layers; ++layer )
	{
		for( unsigned int face = 0; face < CUBEMAP_FACES_NUM; ++face )
		{
			for( unsigned int lvl = 0; lvl < m_levels; ++lvl )
			{
				assert( data[ i ]->Size() == RawFrameSize( lvl ) );
				m_data.push_back( data[ i ] );
				i++;
			}
		}
	}
}

// *********************************
//
SizeType    TextureCubeArray::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level )  * GetHeight( level );
}

// *********************************
//
SizeType    TextureCubeArray::RawFrameSize		( TextureFormat format, UInt32 width, UInt32 height, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level ) * ( height >> level );
}

// *********************************
//
UInt32		TextureCubeArray::GetFacesNum		()
{
	return CUBEMAP_FACES_NUM;
}

// *********************************
//
UInt32		TextureCubeArray::GetIndex			( UInt32 layer, UInt32 face, UInt32 level ) const
{
	return ( layer * CUBEMAP_FACES_NUM * m_levels ) + ( face * m_levels ) + level;
}

} //bv
