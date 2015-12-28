#include "TextureCubeTest.h"

#include <cmath>

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Resources/Textures/TextureCube.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv {

// *****************************
//
TextureCubeTest::TextureCubeTest      ()
	: m_cube( 2.0f )
	, m_texSize( 1024, 1024 )
	, m_angleX( 0.0f )
	, m_angleY( 0.0f )
	, m_texture( nullptr )
	, m_pdrTexture( nullptr )
	, m_sampler( nullptr )
	, m_pdrSampler( nullptr )
{
}

// *****************************
//
TextureCubeTest::~TextureCubeTest     ()
{
	if( m_texture )		delete m_texture;
	if( m_pdrTexture )	delete m_pdrTexture;
	if( m_sampler )		delete m_sampler;
	if( m_pdrSampler )	delete m_pdrSampler;
}

// *****************************
//
void    TextureCubeTest::InitTexture          ()
{
	std::vector< GLubyte > checkImage( m_texSize.x * m_texSize.y * 4 );
	unsigned int numLevels = 1 + ( unsigned int )( log( max( ( double )m_texSize.x, ( double )m_texSize.y ) ) / log ( 2.0 ) );

	GLuint checkboard = Checkboard( &checkImage[ 0 ],  m_texSize.x, m_texSize.y );
	
	m_texture = new TextureCube( TextureFormat::F_A8R8G8B8, m_texSize.x, m_texSize.y, DataBuffer::Semantic::S_STATIC, numLevels ); 
	
	BVGL::bvglBindTexture( GL_TEXTURE_2D, checkboard );

	for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
	{
		auto size = m_texture->RawFrameSize( lvl );
		char * rawData = new char[ size ];
		BVGL::bvglGetTexImage( GL_TEXTURE_2D, lvl, GL_RGBA, GL_UNSIGNED_BYTE, rawData );
		m_texture->SetData( MemoryChunk::Create( rawData, size ), 0, lvl );
	}

	glm::uvec3 colors [] = {
		glm::uvec3( 255, 0, 0 ),
		glm::uvec3( 0, 255, 0 ),
		glm::uvec3( 0, 0, 255 ),
		glm::uvec3( 255, 0, 255 ),
		glm::uvec3( 0, 255, 255 ),
	};

	for( unsigned int face = 0; face < TextureCube::GetFacesNum() - 1; ++face )
	{
		ClearColor( &checkImage[ 0 ], m_texSize.x, m_texSize.y, colors[ face ] );
		for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
		{
			auto size = m_texture->RawFrameSize( lvl );
			char * rawData = new char[ size ];
			memcpy( rawData, &checkImage[ 0 ], size );
			m_texture->SetData( MemoryChunk::Create( rawData, size ), face + 1, lvl );
		}
	}

	m_pdrTexture = PdrTextureCube::Create( m_texture );

    BVGL::bvglDeleteTextures( 1, &checkboard );

	SamplerWrappingMode swm [] = { SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT };
	m_sampler = new TextureSampler( 0, "", SamplerSamplingMode::SSM_MODE_2D, SamplerFilteringMode::SFM_LINEAR_MIPMAP_POINT, swm, glm::vec4( 0, 0, 0, 0 ) );

	m_pdrSampler = new PdrSampler( m_sampler );
}

// *****************************
//
void    TextureCubeTest::Initialize          ( Renderer * renderer, glm::mat4 projMat )
{
	if( m_pdrTexture != nullptr ) return; 

	m_renderer = renderer;

	m_mvp = projMat * glm::translate( glm::mat4(), 0.0f, 0.0f, -5.0f );
	
	InitTexture();

    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

	PrepareShader( "cube", "cube" );
}

// *****************************
//
void    TextureCubeTest::Render              ()
{
	int texUnit = 0;
	m_pdrTexture->Enable( m_renderer, texUnit );
	m_pdrSampler->Bind( texUnit );

    m_program.Use();

	auto mvp = m_mvp * glm::rotate( glm::mat4(), m_angleY, glm::vec3( 1, 0, 0 ) )
		* glm::rotate( glm::mat4(), m_angleX, glm::vec3( 0, 1, 0 ) );

	BVGL::bvglUniformMatrix4fv( 0, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );
	m_cube.Render();
}

// *****************************
//
void    TextureCubeTest::Update              ( TimeType t )
{
	{ t; }
	m_angleX += 0.005f;
	m_angleY += 0.02f;
}

} // bv
