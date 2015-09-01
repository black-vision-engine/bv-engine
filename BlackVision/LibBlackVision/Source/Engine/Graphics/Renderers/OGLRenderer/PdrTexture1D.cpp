#include "PdrTexture1D.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrTexture1D::PdrTexture1D                      ( const Texture2D * texture )
    : m_width( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTexture1D::Initialize      ( const Texture2D * texture )
{
    assert( texture );

    m_txFormat  = texture->GetFormat();

    m_width     = texture->GetWidth();

    //FIXME: allow more texture types here
    assert( m_txFormat == TextureFormat::F_A8R8G8B8 ||
			m_txFormat == TextureFormat::F_R8G8B8 ||
			m_txFormat == TextureFormat::F_A8 || 
			m_txFormat == TextureFormat::F_A32FR32FG32FB32F );

    m_internalFormat    = ConstantsMapper::GLConstantTextureInternalFormat( m_txFormat );
    m_format            = ConstantsMapper::GLConstantTextureFormat( m_txFormat );
    m_type              = ConstantsMapper::GLConstantTextureType( m_txFormat );

    BVGL::bvglGenTextures   ( 1, &m_textureID );
    GLuint prevTex = Bind();

	auto numLevels = texture->GetNumLevels();

	BVGL::bvglTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage1D( GL_TEXTURE_1D, numLevels, m_internalFormat, ( GLsizei )m_width );

	for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
	{
		BVGL::bvglTexSubImage1D( GL_TEXTURE_1D, lvl, 0, ( GLsizei )texture->GetWidth( lvl ), m_format, m_type, texture->GetData( lvl )->Get() );
	}

    BVGL::bvglBindTexture( GL_TEXTURE_1D, prevTex );
}

// *******************************
//
void    PdrTexture1D::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }
}

// *******************************
//
PdrTexture1D::~PdrTexture1D         ()
{
    Deinitialize();
}

// *******************************
//
void        PdrTexture1D::Update            ( const Texture2D * texture )
{
    if ( texture->GetFormat() != m_txFormat || m_width != texture->GetWidth() )
    {
        Deinitialize();
        Initialize( texture );
    }
}

// *******************************
//
GLuint      PdrTexture1D::Bind             ()
{
    GLint current = 0;

    BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_1D, &current );
    BVGL::bvglBindTexture( GL_TEXTURE_1D, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void        PdrTexture1D::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_1D, m_prevTextureID );     
}

// *******************************
//
PdrTexture1D *  PdrTexture1D::Create            ( const Texture2D * texture )
{
    return new PdrTexture1D( texture );
}

} // bv
