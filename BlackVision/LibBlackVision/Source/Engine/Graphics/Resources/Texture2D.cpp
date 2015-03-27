#include "Texture2D.h"

#include <algorithm>

namespace bv {

// *********************************
//
Texture2D::Texture2D                    ( TextureFormat format, SizeType width, SizeType height, DataBuffer::Semantic semantic )
    : Texture( format, TextureType::T_2D, semantic )
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
SizeType    Texture2D::GetWidth         ( UInt32 level ) const
{
	return std::max( m_width >> level, ( SizeType )1 );
}

// *********************************
//
SizeType    Texture2D::GetHeight        ( UInt32 level ) const
{
	return std::max( m_height >> level, ( SizeType )1 );
}

// *********************************
//
SizeType    Texture2D::RawFrameSize		( UInt32 level ) const
{
    return GetPixelSize() * GetWidth( level ) * GetHeight( level );
}

// *********************************
//
SizeType    Texture2D::RawFrameSize		( TextureFormat format, SizeType width, SizeType height, UInt32 level )
{
    return GetPixelSize( format ) * ( width >> level ) * ( height >> level );
}

// *********************************
//
void            Texture2D::SetWidth     ( SizeType width )
{
    m_width = width;
}

// *********************************
//
void            Texture2D::SetHeight    ( SizeType height )
{
    m_height = height;
}

} //bv
