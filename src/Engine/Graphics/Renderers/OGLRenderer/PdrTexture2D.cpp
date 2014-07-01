#include "PdrTexture2D.h"

#include <cassert>

//FIXME: remove
//#include "System/HRTimer.h"

namespace bv
{

//FIXME: remove
//extern HighResolutionTimer GTimer;

// *******************************
// FIXME: implement streaming via two PBOs to make prebuffering "blazingly" fast
// FIXME: implement mipmapping machinery
PdrTexture2D::PdrTexture2D                      ( const Texture2D * texture )
    : m_textureID( 0 )
    , m_prevTextureID( 0 )
    , m_pboMem( nullptr )
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

    auto txSemantic     = texture->GetSemantic();

    if( PdrPBOMemTransfer::PBORequired( txSemantic ) )
    {
        m_pboMem = new PdrPBOMemTransfer( txSemantic, texture->RawFrameSize() );
    }

    glGenTextures   ( 1, &m_textureID );
    GLuint prevTex = Bind();

    if( m_pboMem )
    {
        //NOTE: wystarczy tylko tak, bo update i tak pojdzie dwa razy (raz przy tworzeniu tekstury, a raz przy jej enablowaniu, co oznacza, ze oba PBO zosatana zaladowane poprawnie danymi tekstury i nie bedzie
        //NOTE: jednej pustej ramki z pustym przebiegiem renderera (czyli dokladnie tak, jak byc powinno) - pesymistycznie nalezy tutaj zaladowac od razu jedno PBO, i z niego poprawnie odczytaja sie dane w pierwszym feczu
        glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, 0 );
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, m_type, texture->GetData() );
    }

    glBindTexture( GL_TEXTURE_2D, prevTex );
}

// *******************************
//
void    PdrTexture2D::Deinitialize    ()
{
    if( m_textureID != 0 )
        glDeleteTextures( 1, &m_textureID );
    
    delete m_pboMem;
    m_pboMem = nullptr;
}

// *******************************
//
void    PdrTexture2D::UpdateTexData     ( const Texture2D * texture )
{
    assert( m_pboMem );

    //double writeStart = GTimer.CurElapsed();

    void * data = m_pboMem->LockTexture( MemoryLockingType::MLT_WRITE_ONLY, m_textureID, m_width, m_height, m_format, m_type );
    memcpy( data, texture->GetData(), texture->RawFrameSize() );
    m_pboMem->UnlockTexture( m_textureID, m_width, m_height, m_format, m_type );

    //double writeTime = GTimer.CurElapsed() - writeStart;
    //printf( "Frame streaming took %.4f ms\n", writeTime * 1000.f );
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
void        PdrTexture2D::Update            ( const Texture2D * texture )
{
    if ( texture->GetFormat() != m_txFormat || m_width != texture->GetWidth() || m_height != texture->GetHeight() )
    {
        Deinitialize();
        Initialize( texture );
    }

    if( m_pboMem )
    {
        UpdateTexData( texture );
    }
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
