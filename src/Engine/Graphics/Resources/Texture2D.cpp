#include "Texture2D.h"


namespace bv {

// *********************************
//
Texture2D::Texture2D                    ( TextureFormat format, unsigned int width, unsigned int height, DataBuffer::Semantic semantic )
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
unsigned int    Texture2D::GetWidth         () const
{
    return m_width;
}

// *********************************
//
unsigned int    Texture2D::GetHeight        () const
{
    return m_height;
}

// *********************************
//
unsigned int    Texture2D::RawFrameSize () const
{
    return GetPixelSize() * GetWidth() * GetHeight();
}

// *********************************
//
unsigned int    Texture2D::RawFrameSize ( TextureFormat format, unsigned int width, unsigned int height )
{
    return GetPixelSize( format ) * width * height;
}

// *********************************
//
void            Texture2D::SetWidth     ( unsigned int width )
{
    m_width = width;
}

// *********************************
//
void            Texture2D::SetHeight    ( unsigned int height )
{
    m_height = height;
}

} //bv
