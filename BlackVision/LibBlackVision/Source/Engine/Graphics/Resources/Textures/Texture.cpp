#include "Texture.h"

#include <cstring>

namespace bv
{

//FIXME: implement all functions that Texture 2D should have
int Texture::m_sPixelSize[ TextureFormat::F_TOTAL ] =
{
	16, // F_A32FR32FG32FB32F
	12, // F_R32FG32FB32F			
	4,  // F_A32F					
    4,	// F_A8R8G8B8				
    3,  // F_R8G8B8				
    1,  // F_A8					
    1,  // F_L8					
};

// *********************************
//  
Texture::Texture                                ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic, UInt32 levels, UInt32 dataNum )
    : m_format( format )
    , m_type( type )
    , m_semantic( semantic )
	, m_levels( levels )
	, m_updateID( 1 )
{
	m_data.resize( dataNum, nullptr );
}

// *********************************
//
Texture::~Texture	                            ()
{
}

// *********************************
//
UInt32					Texture::GetNumLevels    () const
{
    return m_levels;
}

// *********************************
//
DataBuffer::Semantic    Texture::GetSemantic    () const
{
    return m_semantic;
}

// *********************************
//
TextureFormat			Texture::GetFormat     () const
{
    return m_format;
}

// *********************************
//
TextureType				Texture::GetType       () const
{
    return m_type;
}

// *********************************
//
SizeType				Texture::GetPixelSize   () const
{
    return GetPixelSize( GetFormat() );
}

// *********************************
//
SizeType				Texture::GetPixelSize   ( TextureFormat format )
{
    return m_sPixelSize[ static_cast< int >( format ) ];
}

// *********************************
//
void					Texture::SetFormat       ( TextureFormat format )
{
    m_format = format;
}

// *********************************
//
void					Texture::SetType         ( TextureType type )
{
    m_type = type;
}

// *********************************
//
MemoryChunkConstPtr		Texture::GetDataChunk		( UInt32 idx ) const
{
	assert( idx < m_data.size() );

	return m_data[ idx ];
}

// *********************************
//
void					Texture::SetDataChunk		( MemoryChunkConstPtr data, UInt32 idx )
{
	assert( idx < m_data.size() );

	m_data[ idx ] = data;
	m_updateID++;
}

// *********************************
//
UInt32                  Texture::GetUpdateID      () const
{
	return m_updateID;
}

// *********************************
//
void					Texture::ForceUpdate      ()
{
	m_updateID++;
}

} //bv
