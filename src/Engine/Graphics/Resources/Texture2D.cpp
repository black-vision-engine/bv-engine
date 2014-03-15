#include "Texture2D.h"


namespace bv {

// *********************************
//
Texture2D::Texture2D                    ( TextureFormat format, int width, int height, DataBuffer::Semantic semantic )
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
int         Texture2D::GetWidth         () const
{
    return m_width;
}

// *********************************
//
int         Texture2D::GetHeight        () const
{
    return m_height;
}

// *********************************
//
unsigned int    Texture2D::RawFrameSize () const
{
    return GetPixelSize() * GetWidth() * GetHeight();
}

} //bv
