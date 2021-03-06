#include "stdafx.h"

#include "PdrTextureCubeArray.h"

#include <cassert>



#include "Memory/MemoryLeaks.h"



namespace bv
{

// *******************************
//
PdrTextureCubeArray::PdrTextureCubeArray                      ( const TextureCubeArray * textureArray )
    : m_width( 0 )
	, m_height( 0 )
	, m_layers( 0 )
{
    Update( textureArray );
}

// *******************************
//
void    PdrTextureCubeArray::Initialize      ( const TextureCubeArray * textureArray )
{
    assert( textureArray );

    m_txFormat  = textureArray->GetFormat();

    m_width     = textureArray->GetWidth();
    m_height    = textureArray->GetHeight();

	m_layers	= textureArray->GetNumLayers();
	
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

	auto numLevels		= textureArray->GetNumLevels();
	auto numFaces		= TextureCubeArray::GetFacesNum();

	auto txSemantic     = textureArray->GetSemantic();
    if( PdrPBOMemTransfer::PBORequired( txSemantic ) )
    {
		m_pboMem.reserve( m_layers * numFaces * numLevels );
		for ( unsigned int layer = 0; layer < m_layers; ++layer )
		{
			for ( unsigned int face = 0; face < numFaces; ++face )
			{
				for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
				{
					m_pboMem.push_back( std::unique_ptr< PdrUploadPBO >( new PdrUploadPBO( txSemantic, textureArray->RawFrameSize( lvl ), false ) ) );
				}
			}
		}
    }

	BVGL::bvglTexParameteri( GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	auto faces = TextureCubeArray::GetFacesNum();
	BVGL::bvglTexStorage3D( GL_TEXTURE_CUBE_MAP_ARRAY, numLevels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_height, ( GLsizei )m_layers * faces );
	
	if( m_pboMem.empty() )
	{
		for( unsigned int layer = 0; layer < m_layers; ++layer )
		{
			for ( unsigned int face = 0; face < faces; ++face )
			{
				for ( unsigned int lvl = 0; lvl < numLevels; ++lvl )
				{
					auto data = textureArray->GetData( layer, face, lvl );
					if( data )
					{
						BVGL::bvglTexSubImage3D( GL_TEXTURE_CUBE_MAP_ARRAY, lvl, 0, 0, ( GLint )layer * faces + face,
							( GLsizei )textureArray->GetWidth( lvl ), ( GLsizei )textureArray->GetHeight( lvl ), GLsizei( 1 ),
							m_format, m_type, data->Get() );
					}
				}
			}
		}
	}

    BVGL::bvglBindTexture( GL_TEXTURE_CUBE_MAP_ARRAY, prevTex );
}

// *******************************
//
void    PdrTextureCubeArray::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }
	m_pboMem.clear();
}

// *******************************
//
PdrTextureCubeArray::~PdrTextureCubeArray         ()
{
    Deinitialize();
}

// *******************************
//
void		PdrTextureCubeArray::UpdateTexData     ( const TextureCubeArray * textureArray )
{
    assert( !m_pboMem.empty() );

	auto numLevels = textureArray->GetNumLevels();
	auto numFaces = TextureCubeArray::GetFacesNum();
	for( unsigned int layer = 0; layer < m_layers; ++layer )
	{
		for( unsigned int face = 0; face < numFaces; ++face )
		{
			for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
			{
				unsigned int idx = layer * numFaces * numLevels + face * numLevels + lvl;
				m_pboMem[ idx ]->LockUpload( textureArray->GetData( layer, face, lvl )->Get(), textureArray->RawFrameSize( lvl ) );
				PBOUploadData( textureArray, layer, face, lvl );
				m_pboMem[ idx ]->UnlockUpload();
			}
		}
	}
}

// *******************************
//
void		PdrTextureCubeArray::PBOUploadData     ( const TextureCubeArray * textureArray, UInt32 layer, UInt32 face, UInt32 lvl )
{
	GLint prevTex = Bind();
	BVGL::bvglTexSubImage3D( GL_TEXTURE_CUBE_MAP_ARRAY, lvl, 0, 0, ( GLint )layer * TextureCubeArray::GetFacesNum() + face,
		( GLsizei )textureArray->GetWidth( lvl ), ( GLsizei )textureArray->GetHeight( lvl ), GLsizei( 1 ),
		m_format, m_type, 0 );
	BVGL::bvglBindTexture( GL_TEXTURE_CUBE_MAP_ARRAY, prevTex );
}

// *******************************
//
void        PdrTextureCubeArray::Update            ( const TextureCubeArray * textureArray )
{
	if ( textureArray->GetFormat() != m_txFormat || 
		m_width != textureArray->GetWidth() ||
		m_height != textureArray->GetHeight() ||
		m_layers != textureArray->GetNumLayers() )
    {
        Deinitialize();
        Initialize( textureArray );
    }

	if( !m_pboMem.empty() )
    {
        UpdateTexData( textureArray );
    }
}

// *******************************
//
GLuint      PdrTextureCubeArray::Bind             ()
{
    GLint current = 0;

    BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_CUBE_MAP_ARRAY, &current );
    BVGL::bvglBindTexture( GL_TEXTURE_CUBE_MAP_ARRAY, m_textureID );

	//BVGL::bvglEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

    return (GLuint) current;
}

// *******************************
//
void        PdrTextureCubeArray::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_CUBE_MAP_ARRAY, m_prevTextureID );     
	//BVGL::bvglDisable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
}

// *******************************
//
PdrTextureCubeArray *  PdrTextureCubeArray::Create            ( const TextureCubeArray * textureArray )
{
    return new PdrTextureCubeArray( textureArray );
}

} // bv
