#include "PdrTexture1DArray.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrTexture1DArray::PdrTexture1DArray                      ( const Texture1DArray * textureArray )
    : m_width( 0 )
	, m_layers( 0 )
{
    Update( textureArray );
}

// *******************************
//
void    PdrTexture1DArray::Initialize      ( const Texture1DArray * textureArray )
{
    assert( textureArray );

    m_txFormat  = textureArray->GetFormat();

    m_width     = textureArray->GetWidth();
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
				m_pboMem.push_back( std::unique_ptr< PdrUploadPBO >( new PdrUploadPBO( txSemantic, textureArray->RawFrameSize( lvl ) ) ) );
			}
		}
    }

	BVGL::bvglTexParameteri( GL_TEXTURE_1D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage2D( GL_TEXTURE_1D_ARRAY, numLevels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_layers );

	if( m_pboMem.empty() )
	{
		for( unsigned int layer = 0; layer < m_layers; ++layer )
		{
			for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
			{
				auto data = textureArray->GetData( layer, lvl );
				if( data )
				{
					BVGL::bvglTexSubImage2D( GL_TEXTURE_1D_ARRAY, lvl, 0, ( GLint )layer,
						( GLsizei )textureArray->GetWidth( lvl ), GLsizei( 1 ),
						m_format, m_type, data->Get() );
				}
			}
		}
	}

    BVGL::bvglBindTexture( GL_TEXTURE_1D_ARRAY, prevTex );
}

// *******************************
//
void    PdrTexture1DArray::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }
	m_pboMem.clear();
}

// *******************************
//
PdrTexture1DArray::~PdrTexture1DArray         ()
{
    Deinitialize();
}

// *******************************
//
void		PdrTexture1DArray::UpdateTexData     ( const Texture1DArray * textureArray )
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
void		PdrTexture1DArray::PBOUploadData     ( const Texture1DArray * textureArray, UInt32 layer, UInt32 lvl )
{
	GLint prevTex = Bind();
	BVGL::bvglTexSubImage2D( GL_TEXTURE_1D_ARRAY, lvl, 0, ( GLint )layer,
		( GLsizei )textureArray->GetWidth( lvl ), GLsizei( 1 ), m_format, m_type, 0 );
	BVGL::bvglBindTexture( GL_TEXTURE_1D_ARRAY, prevTex );
}

// *******************************
//
void        PdrTexture1DArray::Update            ( const Texture1DArray * textureArray )
{
    if ( textureArray->GetFormat() != m_txFormat || 
		m_width != textureArray->GetWidth() || 
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
GLuint      PdrTexture1DArray::Bind             ()
{
    GLint current = 0;

	BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_1D_ARRAY, &current );
    BVGL::bvglBindTexture( GL_TEXTURE_1D_ARRAY, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void        PdrTexture1DArray::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_1D_ARRAY, m_prevTextureID );     
}

// *******************************
//
PdrTexture1DArray *  PdrTexture1DArray::Create            ( const Texture1DArray * textureArray )
{
    return new PdrTexture1DArray( textureArray );
}

} // bv
