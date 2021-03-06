#include "FBOProfilingPrototype2.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "LibImage.h"

#include "Prototypes/FBOProfiling/CacheMissBenchmark.h"
#include "Tools/HRTimer.h"

#include <iostream>


namespace bv {

extern HighResolutionTimer GTimer;

// **************************
//
FBOProfilingPrototype2::FBOProfilingPrototype2    ( Renderer * renderer )
	: FBOProfilingPrototypeBase( renderer )
	, m_currIndex( 0 )
{
	if( !PrepareReadBackBuffers() )
    {
        exit( 1 );
    }

    if( !PrepareShader() )
    {
        exit( 1 );
    }

	m_rawFrameSize = Texture2D::RawFrameSize( TextureFormat::F_A8R8G8B8, m_width, m_height );
	m_buffer = MemoryChunk::Create( m_rawFrameSize );
	GTimer.StartTimer();
}

// **************************
//
FBOProfilingPrototype2::~FBOProfilingPrototype2   ()
{
}

// **************************
//
void    FBOProfilingPrototype2::Initialize     ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// **************************
//
void    FBOProfilingPrototype2::Update         ( TimeType t )
{
    float r = ( cos( t ) + 1.f ) * 0.5f;
    float g = ( sin( t ) + 1.f ) * 0.5f;
    float b = ( cos( t / 3.f ) * sin( t / 2.f ) + 1.f ) * 0.5f;

    m_prog.SetUniform( "color", glm::vec4( r, g, b, 1.f ) );
}

// **************************
//
void    FBOProfilingPrototype2::Render         ()
{
	if( m_enableOffscreenRender )
	{
		Enable();
	}

    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_prog.Use();
	m_rct.Render();

	BVGL::bvglBindTexture( GL_TEXTURE_2D, m_texIds[ 0 ] );
	BVGL::bvglGenerateMipmap( GL_TEXTURE_2D );

	auto err = BVGL::bvglGetError();
	err = err;

	if( m_enableOffscreenRender )
	{
		Disable();

		//auto startTime = GTimer.CurElapsed();
		//auto col = ReadColor();
		//auto stopTime = GTimer.CurElapsed();
		//std::cout << "One frame time (read back) is: " << stopTime - startTime << std::endl;
	}

	m_currFrame++;
}

// **************************
//
void    FBOProfilingPrototype2::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;

	m_enableOffscreenRender = !m_enableOffscreenRender;
}

// **************************
//
void    FBOProfilingPrototype2::Resize         ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// **************************
//
bool    FBOProfilingPrototype2::PrepareShader  ()
{
    std::string shadersRoot = config::PROTOTYPES_SHADERS_ROOT + "FBOProfilingPrototype/";

    std::string vsFile = shadersRoot + "basic_color.vert";
    std::string psFile = shadersRoot + "basic_color.frag";

    if( !m_prog.CompileShaderFromFile( vsFile.c_str(), GL_VERTEX_SHADER ) )
    {
        printf( "Vertex shader failed to compile!\n%s", m_prog.Log().c_str());

        return false;
    }

    if( !m_prog.CompileShaderFromFile( psFile.c_str(), GL_FRAGMENT_SHADER ) )
    {
        printf( "Pixel shader failed to compile!\n%s", m_prog.Log().c_str());

        return false;
    }

    if( !m_prog.Link() )
    {
        printf( "Shader program failed to link!\n%s", m_prog.Log().c_str() );

        return false;
    }

    m_prog.Use();

    return true;
}

// **************************
//
void    FBOProfilingPrototype2::Enable()
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );
	auto err = BVGL::bvglGetError();
	BVGL::bvglDrawBuffers( ( GLsizei )1, &m_drawBuff );
	err = BVGL::bvglGetError();


	BVGL::bvglBindTexture( GL_TEXTURE_2D, m_texIds[ 0 ] );
	BVGL::bvglGenerateMipmap( GL_TEXTURE_2D );
	err = BVGL::bvglGetError();
	err = err;
}

// **************************
//
void    FBOProfilingPrototype2::Disable()
{
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// **************************
//
bool    FBOProfilingPrototype2::PrepareReadBackBuffers()
{
	BVGL::bvglGenBuffers( 1, m_pboID );
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ 0 ] );
    BVGL::bvglBufferData( GL_PIXEL_PACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_READ );

    BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );

	BVGL::bvglGenFramebuffers( 1, &m_fboID );
	m_drawBuff = GL_COLOR_ATTACHMENT0;

    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

	AddTextureForAttachment( 0 );

	//AddTextureForAttachment( 1 );

	BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texIds[ 0 ], 0 );
	//BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_texIds[ 1 ], 0 );

	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );
	if( !CheckFramebuffersStatus() )
	{
		return false;
	}

	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );
	if( !CheckFramebuffersStatus() )
	{
		return false;
	}

    return true;
}

// ****************************
//
bool    FBOProfilingPrototype2::CheckFramebuffersStatus()
{
	auto status = BVGL::bvglCheckFramebufferStatus( GL_FRAMEBUFFER );

    if( status != GL_FRAMEBUFFER_COMPLETE )
    {
        printf( "FBO creation failed with status: %d\n", status );

        return false;
    }

	return true;
}

// ****************************
//
void *   FBOProfilingPrototype2::LockFrameBuffer( SizeType i )
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

	BVGL::bvglReadBuffer( m_fboID );

	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ i ] );
	BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texIds[ i ], 0 );
	BVGL::bvglReadPixels( 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ ( i + 1 ) % 2 ] );
	BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texIds[ ( i + 1 ) % 2 ], 0 );

	void * memory = BVGL::bvglMapBuffer( GL_PIXEL_PACK_BUFFER, GL_READ_WRITE );

	return memory;
}

// ****************************
//
void    FBOProfilingPrototype2::UnlockFrameBuffer  ( SizeType i )
{
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ i ] );
    BVGL::bvglUnmapBuffer( GL_PIXEL_PACK_BUFFER );
    BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
}

// ****************************
//
MemoryChunkConstPtr FBOProfilingPrototype2::ReadColor( )
{
	auto data = LockFrameBuffer( m_currIndex );
	{data;};
	//memcpy( m_buffer->GetWritable(), data, m_rawFrameSize );

	m_currIndex = ( m_currIndex + 1 ) % 2;
	UnlockFrameBuffer( m_currIndex );

	Disable();

	//image::SaveBMPImage( std::to_string( m_currFrame ) + ".bmp", m_buffer, m_width, m_height, 32 );

	return m_buffer;
}

// ****************************
//
void	FBOProfilingPrototype2::AddTextureForAttachment( SizeType i )
{
	auto tx = new Texture2D( TextureFormat::F_A8R8G8B8, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC, 4 );
	tx->SetData( MemoryChunk::Create( m_width * m_height * 4 ), 0 );
	tx->SetData( MemoryChunk::Create( m_width * m_height * 4 / 4 ), 1 );
	tx->SetData( MemoryChunk::Create( m_width * m_height * 4 / 16 ), 2 );
	tx->SetData( MemoryChunk::Create( m_width * m_height * 4 / 64 ), 3 );
    assert( !m_renderer->IsRegistered( tx ) );

    PdrTexture2D * pdrTx = PdrTexture2D::Create( tx );
    m_renderer->RegisterTexture2D( tx, pdrTx );

    //m_drawBuff[ i ] = GL_COLOR_ATTACHMENT0 + i;

    BVGL::bvglBindTexture( GL_TEXTURE_2D, pdrTx->GetTextureID() );
	auto err = BVGL::bvglGetError();
	err = err;
    //FIXME: no mipmaps here
    //FIXME: only NEAREST filters used here - should be just fine, but some implementations use linear filtering for some reasons here
    BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	err = BVGL::bvglGetError();
	err = err;

	BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
	BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  3 );

	m_texIds[ i ] = pdrTx->GetTextureID();
}

// **************************
//
IBasicLogicUnqPtr    FBOProfilingPrototype2::Create  ( Renderer * renderer, audio::AudioRenderer * )
{
	return IBasicLogicUnqPtr( new FBOProfilingPrototype2( renderer ) );
}


} // bv
