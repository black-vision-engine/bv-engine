#include "stdafx.h"

#include "Texture3D.h"

namespace bv {

// *********************************
//
Texture3D::Texture3D                    ( TextureFormat format, UInt32 width, UInt32 height, UInt32 depth, DataBuffer::Semantic semantic, UInt32 levels )
    : Texture( format, TextureType::T_3D, semantic, levels, levels )
    , m_width( width )
    , m_height( height )
    , m_depth( depth )
{
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
	return GetDataChunk( level );
}

// *********************************
//
void				    Texture3D::SetData      ( MemoryChunkConstPtr data, UInt32 level ) 
{
	assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, level );
}

// *********************************
//
void				    Texture3D::SetData		( const std::vector< MemoryChunkConstPtr > & data )
{
	assert( ( UInt32 )data.size() == m_levels );

	m_data.clear();
	for( unsigned int lvl = 0; lvl < m_levels; ++lvl )
	{
		assert( data[ lvl ]->Size() == RawFrameSize( lvl ) );
		m_data.push_back( data[ lvl ] );
	}
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
