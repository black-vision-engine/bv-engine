#include "Texture2D.h"


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
SizeType    Texture2D::GetWidth         () const
{
    return m_width;
}

// *********************************
//
SizeType    Texture2D::GetHeight        () const
{
    return m_height;
}

// *********************************
//
SizeType    Texture2D::RawFrameSize () const
{
    return GetPixelSize() * GetWidth() * GetHeight();
}

// *********************************
//
SizeType    Texture2D::RawFrameSize ( TextureFormat format, SizeType width, SizeType height )
{
    return GetPixelSize( format ) * width * height;
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
