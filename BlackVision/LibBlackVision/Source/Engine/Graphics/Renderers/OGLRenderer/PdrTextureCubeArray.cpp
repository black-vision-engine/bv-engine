#include "PdrTextureCubeArray.h"

#include <cassert>

namespace bv
{

// *******************************
//
PdrTextureCubeArray::PdrTextureCubeArray                      ( const Texture2D * texture )
    : m_width( 0 )
	, m_height( 0 )
	, m_layers( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTextureCubeArray::Initialize      ( const Texture2D * textureArray )
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

	auto numLevels = textureArray->GetNumLevels();
	auto numFaces = textureArray->GetNumFaces();

	BVGL::bvglTexParameteri( GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAX_LEVEL, GLint( numLevels - 1 ) );

	BVGL::bvglTexStorage3D( GL_TEXTURE_CUBE_MAP_ARRAY, numLevels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_height, ( GLsizei )m_layers );

	unsigned int numLayers = textureArray->GetNumLayers();
	for( unsigned int layer = 0; layer < numLayers; ++layer )
	{
		for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
		{
			BVGL::bvglTexSubImage3D( GL_TEXTURE_CUBE_MAP_ARRAY, lvl, 0, 0, ( GLint )layer, 
				( GLsizei )textureArray->GetWidth( lvl ), ( GLsizei )textureArray->GetHeight( lvl ), GLsizei( numFaces ),
				m_format, m_type, textureArray->GetData( lvl, layer )->Get() );
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
}

// *******************************
//
PdrTextureCubeArray::~PdrTextureCubeArray         ()
{
    Deinitialize();
}

// *******************************
//
void        PdrTextureCubeArray::Update            ( const Texture2D * texture )
{
	if ( texture->GetFormat() != m_txFormat || m_width != texture->GetWidth() || m_height != texture->GetHeight() )
    {
        Deinitialize();
        Initialize( texture );
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
PdrTextureCubeArray *  PdrTextureCubeArray::Create            ( const Texture2D * texture )
{
    return new PdrTextureCubeArray( texture );
}

} // bv
