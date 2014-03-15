#include "PdrTextureAnimatedSequence2D.h"

#include <cassert>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv
{

// *******************************
// FIXME: implement streaming via two PBOs to make prebuffering "blazingly" fast
PdrTextureAnimatedSequence2D::PdrTextureAnimatedSequence2D                      ( const Texture2D * texture )
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

    m_width  = texture->GetWidth();
    m_height = texture->GetHeight();

    //FIXME: allow more texture types here (not only RGBA - 8 bit per component)
    assert( texture->GetFormat() == TextureFormat::F_A8R8G8B8 );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    glBindTexture( GL_TEXTURE_2D, prevTex );

    Update( texture );
}

// *******************************
//
PdrTextureAnimatedSequence2D::~PdrTextureAnimatedSequence2D     ()
{
    glDeleteTextures( 1, &m_textureID );
    glDeleteBuffers( 1, &m_pboID );
}

// *******************************
//
void            PdrTextureAnimatedSequence2D::Enable            ( Renderer * renderer, int textureUnit )
{
    glActiveTexture( GL_TEXTURE0 + textureUnit );
    m_prevTextureID = Bind();
}

// *******************************
//
void            PdrTextureAnimatedSequence2D::Disable           ( Renderer * renderer, int textureUnit )
{
    glActiveTexture ( GL_TEXTURE0 + textureUnit );
    Unbind();
}

// *******************************
//
void *          PdrTextureAnimatedSequence2D::Lock            ( MemoryLockingType mlt )
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
void            PdrTextureAnimatedSequence2D::Unlock          ()
{
    if( m_lockedMemoryPtr )
    {
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboID );
        glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER );
    
        if( m_writeLock )
        {
            GLuint prevTex = Bind();

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
void            PdrTextureAnimatedSequence2D::Update          ( const Texture2D * texture )
{
    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, texture->GetData(), texture->GetDataSize() );
    Unlock();
    texture->SetChanged( false );//FIXME: maybe this should be moved to renderer (to be discussed and decided)
}

// *******************************
//
GLuint            PdrTextureAnimatedSequence2D::Bind             ()
{
    GLint current = 0;

    glGetIntegerv( GL_TEXTURE_BINDING_2D, &current );
    glBindTexture( GL_TEXTURE_2D, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void            PdrTextureAnimatedSequence2D::Unbind            ()
{
    glBindTexture   ( GL_TEXTURE_2D, m_prevTextureID );     
}

// *******************************
//
PdrTextureAnimatedSequence2D *   PdrTextureAnimatedSequence2D::Create            ( const Texture2D * texture )
{
    return new PdrTextureAnimatedSequence2D( texture );
}

} // bv
