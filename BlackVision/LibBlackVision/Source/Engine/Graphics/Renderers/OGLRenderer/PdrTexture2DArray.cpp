#include "stdafx.h"

#include "PdrTexture2DArray.h"

#include <cassert>

namespace bv
{

// *******************************
// 
PdrTexture2DArray::PdrTexture2DArray                      ( const Texture2DArray * textureArray )
    : m_width( 0 )
    , m_height( 0 )
	, m_layers( 0 )
{
    Update( textureArray );
}

// *******************************
//
void    PdrTexture2DArray::Initialize      ( const Texture2DArray * textureArray )
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

    auto txSemantic     = textureArray->GetSemantic();
	if( PdrPBOMemTransfer::PBORequired( txSemantic ) )
    {
		m_pboMem.reserve( m_layers * numLevels );
		for( unsigned int layer = 0; layer < m_layers; ++layer )
		{
			for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
			{
				m_pboMem.push_back( std::unique_ptr< PdrUploadPBO >( new PdrUploadPBO( txSemantic, textureArray->RawFrameSize( lvl ), false ) ) );
			}
		}
    }

	BVGL::bvglTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage3D( GL_TEXTURE_2D_ARRAY, numLevels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_height, ( GLsizei )m_layers );

	if( m_pboMem.empty() )
	{
		for( unsigned int layer = 0; layer < m_layers; ++layer )
		{
			for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
			{
				auto data = textureArray->GetData( layer, lvl );
				if( data )
				{
					BVGL::bvglTexSubImage3D( GL_TEXTURE_2D_ARRAY, lvl, 0, 0, ( GLint )layer,
						( GLsizei )textureArray->GetWidth( lvl ), ( GLsizei )textureArray->GetHeight( lvl ), GLsizei( 1 ),
						m_format, m_type, data->Get() );
				}
			}
		}
	}
    BVGL::bvglBindTexture( GL_TEXTURE_2D_ARRAY, prevTex );
}

// *******************************
//
void    PdrTexture2DArray::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }

	m_pboMem.clear();
}

// *******************************
//
void    PdrTexture2DArray::UpdateTexData     ( const Texture2DArray * textureArray )
{
    assert( !m_pboMem.empty() );

	auto numLevels = textureArray->GetNumLevels();
	for( unsigned int layer = 0; layer < m_layers; ++layer )
	{
		for( unsigned int lvl = 0; lvl < numLevels; ++lvl )
		{
			unsigned int idx = layer * numLevels + lvl;
			m_pboMem[ idx ]->LockUpload( textureArray->GetData( layer, lvl )->Get(), textureArray->RawFrameSize( lvl ) );
			PBOUploadData( textureArray, layer, lvl );
			m_pboMem[ idx ]->UnlockUpload();
		}
	}
}

// *******************************
//
void    PdrTexture2DArray::PBOUploadData     ( const Texture2DArray * textureArray, UInt32 layer, UInt32 lvl )
{
	GLint prevTex = Bind();
	BVGL::bvglTexSubImage3D( GL_TEXTURE_2D_ARRAY, lvl, 0, 0, ( GLint )layer,
			( GLsizei )textureArray->GetWidth( lvl ), ( GLsizei )textureArray->GetHeight( lvl ), GLsizei( 1 ),
			m_format, m_type, 0 );
	BVGL::bvglBindTexture( GL_TEXTURE_2D_ARRAY, prevTex );
}

// *******************************
//
PdrTexture2DArray::~PdrTexture2DArray         ()
{
    Deinitialize();
}

// *******************************
//
void        PdrTexture2DArray::Update            ( const Texture2DArray * textureArray )
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
GLuint      PdrTexture2DArray::Bind             ()
{
    GLint current = 0;

	BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_2D_ARRAY, &current );
    BVGL::bvglBindTexture( GL_TEXTURE_2D_ARRAY, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void        PdrTexture2DArray::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_2D_ARRAY, m_prevTextureID );     
}

// *******************************
//
PdrTexture2DArray *  PdrTexture2DArray::Create            ( const Texture2DArray * textureArray )
{
    return new PdrTexture2DArray( textureArray );
}

} // bv
