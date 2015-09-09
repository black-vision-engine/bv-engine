#include "Texture2DArrayTest.h"

#include <cmath>

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Resources/Textures/Texture2DArray.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv {

// *****************************
//
Texture2DArrayTest::Texture2DArrayTest      ()
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
Texture2DArrayTest::~Texture2DArrayTest     ()
{
	if( m_texture )		delete m_texture;
	if( m_pdrTexture )	delete m_pdrTexture;
	if( m_sampler )		delete m_sampler;
	if( m_pdrSampler )	delete m_pdrSampler;
}

// *****************************
//
void    Texture2DArrayTest::InitTexture          ()
{
	std::vector< GLubyte > checkImage( m_texSize.x * m_texSize.y * 4 );
	unsigned int numLevels = 1 + ( unsigned int )( log( max( ( double )m_texSize.x, ( double )m_texSize.y ) ) / log ( 2.0 ) );
	
	GLuint checkboard = Checkboard( &checkImage[ 0 ],  m_texSize.x, m_texSize.y );
	m_texture = new Texture2DArray( TextureFormat::F_A8R8G8B8, m_texSize.x, m_texSize.y, DataBuffer::Semantic::S_STATIC, 
		m_numLayer, numLevels ); 

	BVGL::bvglBindTexture( GL_TEXTURE_2D, checkboard );
	for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
	{
		auto size = m_texture->RawFrameSize( lvl );
		char * rawData = new char[ size ];
		BVGL::bvglGetTexImage( GL_TEXTURE_2D, lvl, GL_RGBA, GL_UNSIGNED_BYTE, rawData );
		m_texture->SetData( MemoryChunk::Create( rawData, size ), 0, lvl );
	}

	//-----
	glm::uvec3 colors [] = {
		glm::uvec3( 255, 0, 0 ),
		glm::uvec3( 0, 255, 0 ),
		glm::uvec3( 0, 0, 255 )
	};

	for( unsigned int i = 0; i < 3; ++i )
	{
		ClearColor( &checkImage[ 0 ], m_texSize.x, m_texSize.y, colors[ i ] );
		for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
		{
			auto size = m_texture->RawFrameSize( lvl );
			char * rawData = new char[ size ];
			memcpy( rawData, &checkImage[ 0 ], size );
			m_texture->SetData( MemoryChunk::Create( rawData, size ), i + 1, lvl );
		}
	}
	//-----

	m_pdrTexture = PdrTexture2DArray::Create( m_texture );

    BVGL::bvglDeleteTextures( 1, &checkboard );


	SamplerWrappingMode swm [] = { SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT };
	m_sampler = new TextureSampler( 0, "", SamplerSamplingMode::SSM_MODE_2D, SamplerFilteringMode::SFM_LINEAR_MIPMAP_POINT, swm, glm::vec4( 0, 0, 0, 0 ) );

	m_pdrSampler = new PdrSampler( m_sampler );
}

// *****************************
//
void    Texture2DArrayTest::Initialize          ( Renderer * renderer, glm::mat4 projMat )
{
	if( m_pdrTexture != nullptr ) return; 

	m_renderer = renderer;

	m_mvp.push_back( projMat * glm::translate( glm::mat4(), 0.0f, -2.0f, -2.0f )
		* glm::rotate( glm::mat4(), 0.0f, glm::vec3( 0, 0, 1 ) )
		* glm::rotate( glm::mat4(), -90.0f, glm::vec3( 1, 0, 0 ) ) );

	m_mvp.push_back( projMat * glm::translate( glm::mat4(), 2.0f, 0.0f, -2.0f )
		* glm::rotate( glm::mat4(), 90.0f, glm::vec3( 0, 0, 1 ) )
		* glm::rotate( glm::mat4(), -90.0f, glm::vec3( 1, 0, 0 ) ) );

	m_mvp.push_back( projMat * glm::translate( glm::mat4(), 0.0f, 2.0f, -2.0f )
		* glm::rotate( glm::mat4(), 180.0f, glm::vec3( 0, 0, 1 ) )
		* glm::rotate( glm::mat4(), -90.0f, glm::vec3( 1, 0, 0 ) ) );

	m_mvp.push_back( projMat * glm::translate( glm::mat4(), -2.0f, 0.0f, -2.0f )
		* glm::rotate( glm::mat4(), 270.0f, glm::vec3( 0, 0, 1 ) )
		* glm::rotate( glm::mat4(), -90.0f, glm::vec3( 1, 0, 0 ) ) );

	InitTexture();

    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

	std::string vsFile = config::PROTOTYPES_SHADERS_ROOT + "TextureTest/" + "defaultArray.vert";
    std::string psFile = config::PROTOTYPES_SHADERS_ROOT + "TextureTest/" + "defaultArray2D.frag";

    m_program.CompileShaderFromFile( vsFile.c_str(), GL_VERTEX_SHADER );
	m_program.CompileShaderFromFile( psFile.c_str(), GL_FRAGMENT_SHADER );
    m_program.Link();
}

// *****************************
//
void    Texture2DArrayTest::Render              ()
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

} // bv