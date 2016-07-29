#include "stdafx.h"

#include "TextureCube.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

const UInt32	TextureCube::CUBEMAP_FACES_NUM = 6;

// *********************************
//
TextureCube::TextureCube                ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 levels )
	: Texture( format, TextureType::T_CUBE, semantic, levels, CUBEMAP_FACES_NUM * levels )
    , m_width( width )
    , m_height( height )
{
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
	return GetDataChunk( GetIndex( face, level ) );
}

// *********************************
//
void				    TextureCube::SetData      ( MemoryChunkConstPtr data, UInt32 face, UInt32 level ) 
{
	assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, GetIndex( face, level ) );
}

// *********************************
//
void				    TextureCube::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	assert( ( UInt32 )data.size() == CUBEMAP_FACES_NUM * m_levels );

	m_data.clear();

	unsigned int i = 0;
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

// *********************************
//
UInt32		TextureCube::GetFacesNum		()
{
	return CUBEMAP_FACES_NUM;
}

// *********************************
//
UInt32		TextureCube::GetIndex			( UInt32 face, UInt32 level ) const
{
	return ( face * m_levels ) + level;
}

} //bv
