#include "stdafx.h"

#include "Texture1D.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
Texture1D::Texture1D                    ( TextureFormat format, UInt32 width, DataBuffer::Semantic semantic, UInt32 levels )
    : Texture( format, TextureType::T_1D, semantic, levels, levels )
    , m_width( width )
{
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
	return GetDataChunk( level );
}

// *********************************
//
void				    Texture1D::SetData      ( MemoryChunkConstPtr data, UInt32 level ) 
{
	assert( data->Size() == RawFrameSize( level ) );
	SetDataChunk( data, level );
}

// *********************************
//
void				    Texture1D::SetData		( const std::vector< MemoryChunkConstPtr > & data )
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
