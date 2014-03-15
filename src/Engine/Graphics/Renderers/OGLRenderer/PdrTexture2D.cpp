#include "PdrTexture2D.h"

#include "Engine/Graphics/Resources/Texture2D.h"

#include <cassert>

namespace bv
{

// *******************************
//FIXME: very simplistic implementation
//FIXME: proper implementation should use PBOs, Locking for memory transfers, and at least a basic MipMapping implementation
PdrTexture2D::PdrTexture2D                      ( const Texture2D * texture )
    : m_textureID( 0 )
    , m_prevTextureID( 0 )
{
    glGenTextures   ( 1, &m_textureID );
    GLuint prevTex = Bind();
  
    //FIXME: allow more texture types here (not only RGBA - 8 bit per component)
    assert( texture->GetFormat() == TextureFormat::F_A8R8G8B8 );
    glTexImage2D    ( GL_TEXTURE_2D, 0, GL_RGBA, texture->GetWidth(), texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetData() );
    glBindTexture   ( GL_TEXTURE_2D, prevTex );

    //FIXME: use PBOs and locking here so that fast texture streaming can be performed
}

// *******************************
//
PdrTexture2D::~PdrTexture2D   ()
{
    glDeleteTextures( 1, &m_textureID );
}

// *******************************
//
void            PdrTexture2D::Enable        ( Renderer * renderer, int textureUnit )
{
    glActiveTexture( GL_TEXTURE0 + textureUnit );
    m_prevTextureID = Bind();
}

// *******************************
//
void            PdrTexture2D::Disable       ( Renderer * renderer, int textureUnit )
{
    glActiveTexture ( GL_TEXTURE0 + textureUnit );
    Unbind();
}

// *******************************
//
void *      PdrTexture2D::Lock              ( MemoryLockingType mlt )
{
}

// *******************************
//
void        PdrTexture2D::Unlock            ()
{
}

// *******************************
//
void        PdrTexture2D::Update            ( const TextureAnimatedSequence2D * texture )
{
}

// *******************************
//
GLuint      PdrTexture2D::Bind             ()
{
    GLint current = 0;

    glGetIntegerv( GL_TEXTURE_BINDING_2D, &current );
    glBindTexture( GL_TEXTURE_2D, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void        PdrTexture2D::Unbind            ()
{
    glBindTexture   ( GL_TEXTURE_2D, m_prevTextureID );     
}

// *******************************
//
PdrTexture2D *   PdrTexture2D::Create            ( const Texture2D * texture )
{
    return new PdrTexture2D( texture );
}

} // bv
