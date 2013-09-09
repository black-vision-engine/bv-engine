#include "Texture.h"

#include "FreeImagePlus.h"
#include <assert.h>

namespace bv
{

//FIXME: implement all functions that Texture 2D should have

int Texture::m_sPixelSize[ Texture::TFormat::F_TOTAL ] =
{
    4,   // F_A8R8G8B8
    4,   // F_R32F
    8,   // F_G32R32F
    16   // F_A32B32G32R32F,
};

// *********************************
//FIXME: implement poperly
Texture::Texture     ( Texture::TFormat format, int w, int h, int numLevels, DataBuffer::Semantic semantic )
{
    //FIXME: implement and stre all texture parameters into texture (w, h, type, pixel format, semantic) - do not use fipImage to do this
}

// *********************************
//
Texture::Texture	                        ( fipImage* img )
    : m_img( img )
    , m_type( Texture::TType::T_2D )
    , m_format( Texture::TFormat::F_A8R8G8B8 )
    , m_semantic( DataBuffer::Semantic::S_TEXTURE )
{
    //FIXME: add other formats
}

// *********************************
//
Texture::~Texture	                        ()
{
    m_img->clear();

    delete m_img;
}

// *********************************
//
unsigned int	        Texture::Width		() const
{
    return m_img->getWidth();
}

// *********************************
//
unsigned int	        Texture::Height		() const
{
    return m_img->getHeight();
}

// *********************************
//
Texture::TFormat        Texture::Format	    () const
{
    return m_format;
}

// *********************************
//
Texture::TType          Texture::Type       () const
{
    return m_type;
}

// *********************************
//
DataBuffer::Semantic    Texture::Semantic   () const
{
    return m_semantic;
}

// *********************************
//
int                     Texture::PixelSize  () const
{
    return PixelSize( Format() );
}

// *********************************
//
int                     Texture::PixelSize  ( Texture::TFormat format )
{
    return m_sPixelSize[ static_cast< int >( format ) ];
}

//// *********************************
////FIXME: Implement
//void	Texture::GenerateMipmaps	()
//{
//    assert(!"Implement this method");
//}
//
//// *********************************
////FIXME: Implement
//bool	Texture::HasMipmaps			() const
//{
//    assert(!"Implement this method");
//    return false;
//}

// *********************************
//
char *		            Texture::Data	    ()
{
    return (char*) m_img->accessPixels();
}

// *********************************
//
const char *            Texture::Data	    () const
{
    return (char*) m_img->accessPixels();
}

}
