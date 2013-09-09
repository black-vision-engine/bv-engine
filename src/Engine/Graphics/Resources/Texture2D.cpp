#include "Texture2D.h"

namespace bv {

Texture2D::Texture2D                       ( Texture::TFormat format, int w, int h, int numLevels, DataBuffer::Semantic semantic =  DataBuffer::Semantic::S_TEXTURE )
{
}

// *********************************
//FIXME: implement all functions that Texture 2D should have
//FIXME: remove fipImage constructors and use more appropriate constructor with dimensions and pixel descriptor
//FIXME: and use fipImage only through TextureManager to create testures (and explicitely set their parameters there instead of pushing fipImage to texture implementation)
Texture2D::Texture2D           ( fipImage * img )
    : Texture( img )
{
}

// *********************************
//
Texture2D::~Texture2D          ()

}