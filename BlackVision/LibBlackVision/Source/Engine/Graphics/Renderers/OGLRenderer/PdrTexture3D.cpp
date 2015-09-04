#include "PdrTexture3D.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrTexture3D::PdrTexture3D                      ( const Texture3D * texture )
    : m_width( 0 )
	, m_height( 0 )
    , m_depth( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTexture3D::Initialize      ( const Texture3D * texture )
{
    assert( texture );

    m_txFormat  = texture->GetFormat();

    m_width     = texture->GetWidth();
    m_height    = texture->GetHeight();
    m_depth     = texture->GetDepth();

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

	BVGL::bvglTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage3D( GL_TEXTURE_3D, numLevels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_height, ( GLsizei )m_depth );

	for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
	{
		auto data = texture->GetData( lvl )->Get();
		if( data )
		{
			BVGL::bvglTexSubImage3D(GL_TEXTURE_3D, lvl,	0, 0, 0, 
				( GLsizei )texture->GetWidth( lvl ), ( GLsizei )texture->GetHeight( lvl ), ( GLsizei )texture->GetDepth( lvl ),
				m_format, m_type, data );
		}
	}

    BVGL::bvglBindTexture( GL_TEXTURE_3D, prevTex );
}

// *******************************
//
void    PdrTexture3D::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }
}

// *******************************
//
PdrTexture3D::~PdrTexture3D         ()
{
    Deinitialize();
}

// *******************************
//
void        PdrTexture3D::Update            ( const Texture3D * texture )
{
	if ( texture->GetFormat() != m_txFormat || 
		m_width != texture->GetWidth() || 
		m_height != texture->GetHeight() || 
		m_depth != texture->GetDepth() )
    {
        Deinitialize();
        Initialize( texture );
    }
}

// *******************************
//
GLuint      PdrTexture3D::Bind             ()
{
    GLint current = 0;

    BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_3D, &current );
    BVGL::bvglBindTexture( GL_TEXTURE_3D, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void        PdrTexture3D::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_3D, m_prevTextureID );     
}

// *******************************
//
PdrTexture3D *  PdrTexture3D::Create            ( const Texture3D * texture )
{
    return new PdrTexture3D( texture );
}

} // bv
