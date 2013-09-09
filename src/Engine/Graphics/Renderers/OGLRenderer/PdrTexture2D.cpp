#include "Texture.h"

#include <assert.h>


namespace bv
{
//
//namespace
//{
//    GLuint getOGLTextureFormat(Texture::Format format)
//    {
//        switch(format)
//        {
//        case Texture::Format::A8R8G8B8:
//            return GL_BGRA;
//        default:
//            assert(!"Implement");
//        }
//
//        return 0;
//    }
//} // anonymouse
//
//PdrTexture::PdrTexture(const Texture* tex)
//    : m_textureName(0)
//    , m_isBinded(false)
//    , m_width(tex->getWidth())
//    , m_height(tex->getHeight())
//    , m_data(tex->getData())
//    , m_format(getOGLTextureFormat(tex->getFormat()))
//{
//}
//
//GLuint PdrTexture::bind()
//{
//    // Generate texture name.  
//    glGenTextures( 1, &m_textureName);  
//  
//    // Bind texture name.  
//    glBindTexture( GL_TEXTURE_2D, m_textureName );  
//  
//    // Initialise texture with image data.  
//    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height,  
//                0, GL_RGB, GL_UNSIGNED_BYTE, m_data);  
//  
//    // Set minification and magnification filter settings.  
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );  
//
//    m_isBinded = true;
//
//    return m_textureName;
//}
//
//GLuint PdrTexture::getTextureName() const
//{
//    assert(m_isBinded);
//
//    return m_textureName;
//}
//
}