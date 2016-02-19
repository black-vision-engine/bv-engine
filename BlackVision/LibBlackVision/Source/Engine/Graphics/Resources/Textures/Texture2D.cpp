#include "stdafx.h"

#include "Texture2D.h"

namespace bv {

// *********************************
//
Texture2D::Texture2D                    ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, UInt32 levels )
	: Texture( format, TextureType::T_2D, semantic, levels, levels )
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
	return GetDataChunk( level );
}

// *********************************
//
void				    Texture2D::SetData      ( MemoryChunkConstPtr data, UInt32 level ) 
{
    assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, level );
}

// *********************************
//
void				    Texture2D::SetData      ( MemoryChunkConstPtr data, TextureFormat format, UInt32 width, UInt32 height, UInt32 level ) 
{
	m_format = format;
	m_width = width;
	m_height = height;

    SetData( data, level );
}

// *********************************
//
void				    Texture2D::SetData		( const std::vector< MemoryChunkConstPtr > & data, TextureFormat format, UInt32 width, UInt32 height, UInt32 levels )
{
	m_format = format;
	m_width = width;
	m_height = height;
	m_levels = levels;

	m_data.resize( levels );

	for( unsigned int lvl = 0; lvl < m_levels; ++lvl )
	{
		assert( data[ lvl ]->Size() == RawFrameSize( lvl ) );
		m_data[ lvl ] = data[ lvl ];
	}

	m_updateID++;
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
