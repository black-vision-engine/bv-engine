#include "PdrTexture2D.h"

#include "Engine/Graphics/Resources/Texture2D.h"

#include <cassert>

namespace bv
{

// *******************************
// FIXME: implement streaming via two PBOs to make prebuffering "blazingly" fast
// FIXME: implement mipmapping machinery
PdrTexture2D::PdrTexture2D                      ( const Texture2D * texture )
    : m_textureID( 0 )
    , m_prevTextureID( 0 )
    , m_pboID( 0 )
    , m_writeLock( false )
    , m_lockedMemoryPtr( nullptr )
{
    glGenBuffers( 1, &m_pboID );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboID );
    glBufferData( GL_PIXEL_UNPACK_BUFFER, texture->RawFrameSize(), 0, GL_DYNAMIC_DRAW );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );

    glGenTextures   ( 1, &m_textureID );
    GLuint prevTex = Bind();
  
    m_width     = texture->GetWidth();
    m_height    = texture->GetHeight();

    //FIXME: allow more texture types here (not only RGBA - 8 bit per component)
    assert( texture->GetFormat() == TextureFormat::F_A8R8G8B8 );
    glTexImage2D    ( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    glBindTexture   ( GL_TEXTURE_2D, prevTex );

    Update( texture );
}

// *******************************
//
PdrTexture2D::~PdrTexture2D   ()
{
    glDeleteTextures( 1, &m_textureID );
    glDeleteBuffers( 1, &m_pboID );
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
    if( !m_lockedMemoryPtr )
    {
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboID );
        m_lockedMemoryPtr = glMapBuffer( GL_PIXEL_UNPACK_BUFFER, ConstantsMapper::GLConstant( mlt ) );
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER_ARB, 0 );

        m_writeLock = mlt != MemoryLockingType::MLT_READ_ONLY;
    }

    return m_lockedMemoryPtr;
}

// *******************************
//
void        PdrTexture2D::Unlock            ()
{
    if( m_lockedMemoryPtr )
    {
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboID );
        glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER );
    
        if( m_writeLock )
        {
            GLuint prevTex = Bind();

            //This call uploads data from bound currently bound PBO to currently bound texture
            //FIXME: allow more texture types here (not only RGBA - 8 bit per component)
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
            glBindTexture( GL_TEXTURE_2D, prevTex );

            m_writeLock = false;
        }

        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
        m_lockedMemoryPtr = nullptr;
    }
}

// *******************************
//
void        PdrTexture2D::Update            ( const Texture2D * texture )
{
    //FIXME: add pbo regeneration - in case the texture was reloaded
    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, texture->GetData(), texture->RawFrameSize() );
    Unlock();
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
