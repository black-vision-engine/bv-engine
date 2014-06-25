#include "PdrTexture2D.h"

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
    , m_width( 0 )
    , m_height( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTexture2D::Initialize      ( const Texture2D * texture )
{
    assert( texture );

    m_txFormat  = texture->GetFormat();

    m_width     = texture->GetWidth();
    m_height    = texture->GetHeight();

    //FIXME: allow more texture types here
    assert( m_txFormat == TextureFormat::F_A8R8G8B8 || m_txFormat == TextureFormat::F_R8G8B8 || m_txFormat == TextureFormat::F_A8 );

    m_internalFormat    = ConstantsMapper::GLConstantTextureInternalFormat( m_txFormat );
    m_format            = ConstantsMapper::GLConstantTextureFormat( m_txFormat );
    m_type              = ConstantsMapper::GLConstantTextureType( m_txFormat );

    auto semantic       = ConstantsMapper::GLConstant( texture->GetSemantic() );

    glGenBuffers( 1, &m_pboID );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, m_pboID );
    glBufferData( GL_PIXEL_UNPACK_BUFFER, texture->RawFrameSize(), 0, semantic );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );

    glGenTextures   ( 1, &m_textureID );
    GLuint prevTex = Bind();

    glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0 );
    glBindTexture( GL_TEXTURE_2D, prevTex );
}

// *******************************
//
void    PdrTexture2D::Deinitialize    ()
{
    if( m_textureID != 0 )
        glDeleteTextures( 1, &m_textureID );

    if( m_pboID != 0 )
        glDeleteBuffers( 1, &m_pboID );
}

// *******************************
//
void    PdrTexture2D::UpdateTexData     ( const Texture2D * texture )
{
    void * data = Lock( MemoryLockingType::MLT_WRITE_ONLY );
    memcpy( data, texture->GetData(), texture->RawFrameSize() );
    Unlock();
}

// *******************************
//
PdrTexture2D::~PdrTexture2D   ()
{
    Deinitialize();
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
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );

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

            glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0 );
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
    if ( texture->GetFormat() != m_txFormat || m_width != texture->GetWidth() || m_height != texture->GetHeight() )
    {
        Deinitialize();
        Initialize( texture );
    }

    UpdateTexData( texture );
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
GLuint      PdrTexture2D::GetTextureID    () const
{
    return m_textureID;
}

// *******************************
//
PdrTexture2D *  PdrTexture2D::Create            ( const Texture2D * texture )
{
    return new PdrTexture2D( texture );
}

} // bv
