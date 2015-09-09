#include "Texture2DTest.h"

#include <cmath>

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Shaders/TextureSampler.h"

namespace bv {

// *****************************
//
Texture2DTest::Texture2DTest      ()
	: m_rct( 8.0f, 8.0f )
	, m_texSize( 1024, 1024 )
	, m_texture( nullptr )
	, m_pdrTexture( nullptr )
	, m_sampler( nullptr )
	, m_pdrSampler( nullptr )
{
}

// *****************************
//
Texture2DTest::~Texture2DTest     ()
{
	if( m_texture )		delete m_texture;
	if( m_pdrTexture )	delete m_pdrTexture;
	if( m_sampler )		delete m_sampler;
	if( m_pdrSampler )	delete m_pdrSampler;
}

// *****************************
//
void    Texture2DTest::InitTexture          ()
{
	std::vector< GLubyte > checkImage( m_texSize.x * m_texSize.y * 4 );
	unsigned int numLevels = 1 + ( unsigned int )( log( max( ( double )m_texSize.x, ( double )m_texSize.y ) ) / log ( 2.0 ) );

	GLuint checkboard = Checkboard( &checkImage[ 0 ],  m_texSize.x, m_texSize.y );
	
	m_texture = new Texture2D( TextureFormat::F_A8R8G8B8, m_texSize.x, m_texSize.y, DataBuffer::Semantic::S_STATIC, numLevels ); 
	
	BVGL::bvglBindTexture( GL_TEXTURE_2D, checkboard );
	for (unsigned int lvl = 0; lvl < numLevels; ++lvl)
	{
		auto size = m_texture->RawFrameSize( lvl );
		char * rawData = new char[ size ];
		BVGL::bvglGetTexImage( GL_TEXTURE_2D, lvl, GL_RGBA, GL_UNSIGNED_BYTE, rawData );
		m_texture->SetData( MemoryChunk::Create( rawData, size ), lvl );
	}

	m_pdrTexture = PdrTexture2D::Create( m_texture );

    BVGL::bvglDeleteTextures( 1, &checkboard );


	SamplerWrappingMode swm [] = { SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT, SamplerWrappingMode::SWM_REPEAT };
	m_sampler = new TextureSampler( 0, "", SamplerSamplingMode::SSM_MODE_2D, SamplerFilteringMode::SFM_LINEAR_MIPMAP_POINT, swm, glm::vec4( 0, 0, 0, 0 ) );

	m_pdrSampler = new PdrSampler( m_sampler );
}

// *****************************
//
void    Texture2DTest::Initialize          ( Renderer * renderer, glm::mat4 projMat )
{
	if( m_pdrTexture != nullptr ) return; 

	m_renderer = renderer;
	m_mvp = projMat * glm::translate( glm::mat4(), 0.0f, 0.0f, -2.0f ) * 
		glm::rotate( glm::mat4(), -75.0f, glm::vec3( 1, 0, 0 ) );

	InitTexture();

    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

	std::string vsFile = config::PROTOTYPES_SHADERS_ROOT + "TextureTest/" + "default.vert";
    std::string psFile = config::PROTOTYPES_SHADERS_ROOT + "TextureTest/" + "default2D.frag";

    m_program.CompileShaderFromFile( vsFile.c_str(), GL_VERTEX_SHADER );
	m_program.CompileShaderFromFile( psFile.c_str(), GL_FRAGMENT_SHADER );
    m_program.Link();
}

// *****************************
//
void    Texture2DTest::Render              ()
{
	int texUnit = 0;
	m_pdrTexture->Enable( m_renderer, texUnit );
	m_pdrSampler->Bind( texUnit );

    m_program.Use();

	BVGL::bvglUniformMatrix4fv( 0, 1, GL_FALSE, &m_mvp[ 0 ][ 0 ] );

    m_rct.Render();
}

} // bv
