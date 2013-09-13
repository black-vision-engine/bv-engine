#include "Texture2D.h"

namespace bv {

// *********************************
//
            Texture2D::Texture2D        ( Texture::TFormat format, int width, int height, DataBuffer::Semantic semantic )
    : Texture(format, TType::T_2D, semantic)
    , m_width(width)
    , m_height(height)
{}

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
            Texture2D::~Texture2D       ()
{}

}