#include "stdafx.h"

#include "PdrTexture1D.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrTexture1D::PdrTexture1D                      ( const Texture1D * texture )
    : m_width( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTexture1D::Initialize      ( const Texture1D * texture )
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

	auto numLevels			= texture->GetNumLevels();

    auto txSemantic     = texture->GetSemantic();
	if( PdrPBOMemTransfer::PBORequired( txSemantic ) )
    {
		m_pboMem.reserve( numLevels );
		for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
		{
			m_pboMem.push_back( std::unique_ptr< PdrUploadPBO >( new PdrUploadPBO( txSemantic, texture->RawFrameSize( lvl ), false ) ) );
		}
    }

	BVGL::bvglTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage1D( GL_TEXTURE_1D, numLevels, m_internalFormat, ( GLsizei )m_width );

	if( m_pboMem.empty() )
	{
		for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
		{
			auto data = texture->GetData( lvl );
			if( data )
			{
				BVGL::bvglTexSubImage1D( GL_TEXTURE_1D, lvl, 0, ( GLsizei )texture->GetWidth( lvl ), m_format, m_type, data->Get() );
			}
		}
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

	m_pboMem.clear();
}

// *******************************
//
PdrTexture1D::~PdrTexture1D         ()
{
    Deinitialize();
}

// *******************************
//
void		PdrTexture1D::UpdateTexData     ( const Texture1D * texture )
{
    assert( !m_pboMem.empty() );

	for( unsigned int lvl = 0; lvl < texture->GetNumLevels(); ++lvl )
	{
		m_pboMem[ lvl ]->LockUpload( texture->GetData( lvl )->Get(), texture->RawFrameSize( lvl ) );
		PBOUploadData( texture, lvl );
		m_pboMem[ lvl ]->UnlockUpload();
	}
}

// *******************************
//
void		PdrTexture1D::PBOUploadData     ( const Texture1D * texture, UInt32 lvl )
{
	GLint prevTex = Bind();
	BVGL::bvglTexSubImage1D( GL_TEXTURE_1D, lvl, 0, ( GLsizei )texture->GetWidth( lvl ), m_format, m_type, 0 );
	BVGL::bvglBindTexture( GL_TEXTURE_1D, prevTex );
}

// *******************************
//
void        PdrTexture1D::Update            ( const Texture1D * texture )
{
    if ( texture->GetFormat() != m_txFormat ||
		m_width != texture->GetWidth() )
    {
        Deinitialize();
        Initialize( texture );
    }

	if( !m_pboMem.empty() )
    {
        UpdateTexData( texture );
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
PdrTexture1D *  PdrTexture1D::Create            ( const Texture1D * texture )
{
    return new PdrTexture1D( texture );
}

} // bv
