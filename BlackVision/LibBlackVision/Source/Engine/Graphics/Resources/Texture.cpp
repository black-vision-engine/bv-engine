#include "Texture.h"

#include <cassert>
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
Texture::Texture                                ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic, UInt32 layers, UInt32 levels )
    : m_format( format )
    , m_type( type )
    , m_semantic( semantic )
    , m_changed( true )
	, m_layers( layers ) 
	, m_levels( levels ) 
{
	m_data.resize( layers * levels );
}

// *********************************
//
Texture::~Texture	                            ()
{
}

// *********************************
//
UInt32					Texture::GetNumLayers    () const
{
    return m_layers;
}

// *********************************
//
UInt32					Texture::GetNumLevels    () const
{
    return m_levels;
}

// *********************************
//
void					Texture::SetNumLayers    ( UInt32 layers )
{
	m_layers = layers;
}

// *********************************
//
void					Texture::SetNumLevels    ( UInt32 levels )
{
    m_levels = levels;
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
const char *			Texture::GetRawData     ( UInt32 layer, UInt32 level ) const
{
	UInt32 idx = layer * m_levels + level;
	if( idx < m_data.size() )
	{
		return m_data[ idx ]->Get();
	}
	return nullptr;
}

// *********************************
//
MemoryChunkConstPtr		Texture::GetDataChunk		( UInt32 layer, UInt32 level ) const
{
	UInt32 idx = layer * m_levels + level;
	if( idx < m_data.size() )
	{
		return m_data[ idx ];
	}
	return nullptr;
}

// *********************************
//
void					Texture::SetDataChunk		( MemoryChunkConstPtr data, UInt32 layer, UInt32 level )
{
	UInt32 idx = layer * m_levels + level;
	if( idx < m_data.size() )
	{
		m_data[ idx ] = data;
	}
}

// *********************************
//
void                    Texture::SetChanged     ( bool changed ) const
{
    m_changed = changed;
}

// *********************************
//
bool                    Texture::Changed        () const
{
    return m_changed;
}

} //bv
