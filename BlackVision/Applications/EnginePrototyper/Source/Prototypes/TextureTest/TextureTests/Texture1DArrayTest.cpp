#include "Texture1DArrayTest.h"

#include <cmath>

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Resources/Textures/Texture1DArray.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv {

// *****************************
//
Texture1DArrayTest::Texture1DArrayTest      ()
	: m_texSize( 1024, 1024 )
	, m_rct( 4.0f, 4.0f )
	, m_texture( nullptr )
	, m_pdrTexture( nullptr )
	, m_sampler( nullptr )
	, m_pdrSampler( nullptr )
	, m_numLayer( 4 )
{
}

// *****************************
//
Texture1DArrayTest::~Texture1DArrayTest     ()
{
	if( m_texture )		delete m_texture;
	if( m_pdrTexture )	delete m_pdrTexture;
	if( m_sampler )		delete m_sampler;
	if( m_pdrSampler )	delete m_pdrSampler;
}

// *****************************
//
void    Texture1DArrayTest::InitTexture          ()
{
	std::vector< GLubyte > checkImage( m_texSize.x * m_texSize.y * 4 );
	unsigned int numLevels = 1 + ( unsigned int )( log( std::max( ( double )m_texSize.x, ( double )m_texSize.y ) ) / log ( 2.0 ) );
	
	GLuint checkboard = Checkboard( &checkImage[ 0 ],  m_texSize.x, m_texSize.y );
	m_texture = new Texture1DArray( TextureFormat::F_A8R8G8B8, m_texSize.x, DataBuffer::Semantic::S_STATIC, 
		m_numLayer, numLevels ); 

	BVGL::bvglBindTexture( GL_TEXTURE_2D, checkboard );
	for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
	{
		auto size = m_texture->RawFrameSize( lvl );
		char * rawData = new char[ size ];

		char * imgData = new char[ Texture2D::RawFrameSize( TextureFormat::F_A8R8G8B8, m_texSize.x, m_texSize.y, lvl ) ];
		BVGL::bvglGetTexImage( GL_TEXTURE_2D, lvl, GL_RGBA, GL_UNSIGNED_BYTE, imgData );

		memcpy( rawData, imgData, size );
		m_texture->SetData( MemoryChunk::Create( rawData, size ), 0, lvl );
		delete imgData;
	}

	//-----
	glm::uvec3 colors [] = {
		glm::uvec3( 255, 0, 0 ),
		glm::uvec3( 0, 255, 0 ),
		glm::uvec3( 0, 0, 255 )
	};

	for( unsigned int i = 0; i < 3; ++i )
	{
		ClearColor( &checkImage[ 0 ], m_texSize.x, 1, colors[ i ] );
		for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
		{
			auto size = m_texture->RawFrameSize( lvl );
			char * rawData = new char[ size ];
			memcpy( rawData, &checkImage[ 0 ], size );
			m_texture->SetData( MemoryChunk::Create( rawData, size ), i + 1, lvl );
		}
	}
	//-----

	m_pdrTexture = PdrTexture1DArray::Create( m_texture );

    BVGL::bvglDeleteTextures( 1, &checkboard );


	SamplerWrappingMode swm [] = { SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT };
	m_sampler = new TextureSampler( 0, "", SamplerSamplingMode::SSM_MODE_2D, SamplerFilteringMode::SFM_LINEAR_MIPMAP_POINT, swm, glm::vec4( 0, 0, 0, 0 ) );

	m_pdrSampler = new PdrSampler( m_sampler );
}

// *****************************
//
void    Texture1DArrayTest::Initialize          ( Renderer * renderer, glm::mat4 projMat )
{
	if( m_pdrTexture != nullptr ) return; 

	m_renderer = renderer;

	m_mvp.push_back( projMat * glm::translate( glm::mat4(), 0.0f, 0.0f, -2.0f ) * 
		glm::rotate( glm::mat4(), -75.0f, glm::vec3( 1, 0, 0 ) ) );

	m_mvp.push_back( projMat * glm::translate( glm::mat4(), 8.0f, 4.0f, -10.0f ) );
	m_mvp.push_back( projMat * glm::translate( glm::mat4(), 0.0f, 4.0f, -10.0f ) );
	m_mvp.push_back( projMat * glm::translate( glm::mat4(), -8.0f, 4.0f, -10.0f ) );

	InitTexture();

    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

	PrepareShader( "defaultArray", "defaultArray1D" );
}

// *****************************
//
void    Texture1DArrayTest::Render              ()
{
	int texUnit = 0;
	m_pdrTexture->Enable( m_renderer, texUnit );
	m_pdrSampler->Bind( texUnit );

    m_program.Use();

	for( int i = 0; i < m_mvp.size(); ++i )
	{
		BVGL::bvglUniformMatrix4fv( 0, 1, GL_FALSE, &m_mvp[ i ][ 0 ][ 0 ] );
		BVGL::bvglUniform1i( 1, i );
		m_rct.Render();
	}
}

// *****************************
//
void    Texture1DArrayTest::Update              ( TimeType t )
{
	{ t; }
}

} // bv