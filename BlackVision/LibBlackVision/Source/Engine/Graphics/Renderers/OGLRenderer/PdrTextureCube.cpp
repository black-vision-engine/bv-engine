#include "stdafx.h"

#include "PdrTextureCube.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv
{

// *******************************
//
PdrTextureCube::PdrTextureCube                      ( const TextureCube * texture )
    : m_width( 0 )
	, m_height( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTextureCube::Initialize      ( const TextureCube * texture )
{
    assert( texture );

    m_txFormat  = texture->GetFormat();

    m_width     = texture->GetWidth();
    m_height    = texture->GetHeight();

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
	auto numFaces			= TextureCube::GetFacesNum();

	auto txSemantic     = texture->GetSemantic();
    if( PdrPBOMemTransfer::PBORequired( txSemantic ) )
    {
		m_pboMem.reserve( numFaces * numLevels );
		for ( unsigned int face = 0; face < numFaces; ++face )
		{
			for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
			{
				m_pboMem.push_back( std::unique_ptr< PdrUploadPBO >( new PdrUploadPBO( txSemantic, texture->RawFrameSize( lvl ), false ) ) );
			}
		}
    }

	BVGL::bvglTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage2D( GL_TEXTURE_CUBE_MAP, numLevels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_height );

	if( m_pboMem.empty() )
	{
		for ( unsigned int face = 0; face < numFaces; ++face )
		{
			for ( unsigned int lvl = 0; lvl < numLevels; ++lvl )
			{
				auto data = texture->GetData( face, lvl );
				if( data )
				{
					BVGL::bvglTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum( face ), lvl, 0, 0,
						( GLsizei )texture->GetWidth( lvl ), ( GLsizei )texture->GetHeight( lvl ), m_format, m_type, data->Get() );
				}
			}
		}
	}

	BVGL::bvglBindTexture( GL_TEXTURE_CUBE_MAP, prevTex );
}

// *******************************
//
void    PdrTextureCube::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }
	m_pboMem.clear();
}

// *******************************
//
PdrTextureCube::~PdrTextureCube         ()
{
    Deinitialize();
}

// *******************************
//
void		PdrTextureCube::UpdateTexData     ( const TextureCube * texture )
{
    assert( !m_pboMem.empty() );

	auto numLevels = texture->GetNumLevels();
	for( unsigned int face = 0; face < TextureCube::GetFacesNum(); ++face )
	{
		for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
		{
			unsigned int idx = face * numLevels + lvl;
			m_pboMem[ idx ]->LockUpload( texture->GetData( face, lvl )->Get(), texture->RawFrameSize( lvl ) );
			PBOUploadData( texture, face, lvl );
			m_pboMem[ idx ]->UnlockUpload();
		}
	}
}

// *******************************
//
void		PdrTextureCube::PBOUploadData     ( const TextureCube * texture, UInt32 face, UInt32 lvl )
{
	GLint prevTex = Bind();
	BVGL::bvglTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum( face ), lvl, 0, 0,
		( GLsizei )texture->GetWidth( lvl ), ( GLsizei )texture->GetHeight( lvl ), m_format, m_type, 0 );
	BVGL::bvglBindTexture( GL_TEXTURE_CUBE_MAP, prevTex );
}


// *******************************
//
void        PdrTextureCube::Update            ( const TextureCube * texture )
{
	if ( texture->GetFormat() != m_txFormat || 
		m_width != texture->GetWidth() ||
		m_height != texture->GetHeight() )
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
GLuint      PdrTextureCube::Bind             ()
{
    GLint current = 0;

    BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_CUBE_MAP, &current );
    BVGL::bvglBindTexture( GL_TEXTURE_CUBE_MAP, m_textureID );

	//BVGL::bvglEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

    return (GLuint) current;
}

// *******************************
//
void        PdrTextureCube::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_CUBE_MAP, m_prevTextureID );     
	//BVGL::bvglDisable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
}

// *******************************
//
PdrTextureCube *  PdrTextureCube::Create            ( const TextureCube * texture )
{
    return new PdrTextureCube( texture );
}

} // bv
