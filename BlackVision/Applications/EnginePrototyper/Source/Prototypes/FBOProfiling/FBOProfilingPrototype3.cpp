#include "FBOProfilingPrototype3.h"

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
FBOProfilingPrototype3::FBOProfilingPrototype3    ( Renderer * renderer )
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
FBOProfilingPrototype3::~FBOProfilingPrototype3   ()
{
}

// **************************
//
void    FBOProfilingPrototype3::Initialize     ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// **************************
//
void    FBOProfilingPrototype3::Update         ( TimeType t )
{
    float r = ( cos( t ) + 1.f ) * 0.5f;
    float g = ( sin( t ) + 1.f ) * 0.5f;
    float b = ( cos( t / 3.f ) * sin( t / 2.f ) + 1.f ) * 0.5f;

    m_prog.SetUniform( "color", glm::vec4( r, g, b, 1.f ) );
}

// **************************
//
void    FBOProfilingPrototype3::Render         ()
{
	if( m_enableOffscreenRender )
	{
		Enable();
	}

    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_prog.Use();
	m_rct.Render();

	if( m_enableOffscreenRender )
	{
		Disable();

		auto startTime = GTimer.CurElapsed();
		auto col = ReadColor();
		auto stopTime = GTimer.CurElapsed();
		std::cout << "One frame time (read back) is: " << stopTime - startTime << std::endl;
	}

	m_currFrame++;
}

// **************************
//
void    FBOProfilingPrototype3::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;

	m_enableOffscreenRender = !m_enableOffscreenRender;
}

// **************************
//
void    FBOProfilingPrototype3::Resize         ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// **************************
//
bool    FBOProfilingPrototype3::PrepareShader  ()
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
void    FBOProfilingPrototype3::Enable()
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );
	BVGL::bvglDrawBuffers( ( GLsizei )1, &m_drawBuff );
}

// **************************
//
void    FBOProfilingPrototype3::Disable()
{
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// **************************
//
bool    FBOProfilingPrototype3::PrepareReadBackBuffers()
{
	BVGL::bvglGenBuffers( 2, m_pboID );
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ 0 ] );
    BVGL::bvglBufferData( GL_PIXEL_PACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_READ );
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ 1 ] );
    BVGL::bvglBufferData( GL_PIXEL_PACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_READ );

    BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );

	BVGL::bvglGenFramebuffers( 1, &m_fboID );
	m_drawBuff = GL_COLOR_ATTACHMENT0;

    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

	BVGL::bvglGenRenderbuffers( 2, m_renderBuffersIds );

	AddRenderBufferStorage( 0 );

	AddRenderBufferStorage( 1 );

	BVGL::bvglBindRenderbuffer( GL_RENDERBUFFER, m_renderBuffersIds[ 0 ] );

	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );
	if( !CheckFramebuffersStatus() )
	{
		return false;
	}

    return true;
}

// ****************************
//
bool    FBOProfilingPrototype3::CheckFramebuffersStatus()
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
void *   FBOProfilingPrototype3::LockFrameBuffer( SizeType i )
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

	BVGL::bvglReadBuffer( m_fboID );

	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ i ] );
	BVGL::bvglBindRenderbuffer( GL_RENDERBUFFER, m_renderBuffersIds[ i ] );

	BVGL::bvglFramebufferRenderbuffer(	GL_FRAMEBUFFER,      
										GL_COLOR_ATTACHMENT0, 
										GL_RENDERBUFFER,
										m_renderBuffersIds[ i ] );

	BVGL::bvglReadPixels( 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ ( i + 1 ) % 2 ] );
	BVGL::bvglBindRenderbuffer( GL_RENDERBUFFER, m_renderBuffersIds[ ( i + 1 ) % 2 ] );

	BVGL::bvglFramebufferRenderbuffer(	GL_FRAMEBUFFER,      
										GL_COLOR_ATTACHMENT0, 
										GL_RENDERBUFFER,
										m_renderBuffersIds[ ( i + 1 ) % 2 ] );

	void * memory = BVGL::bvglMapBuffer( GL_PIXEL_PACK_BUFFER, GL_READ_WRITE );

	return memory;
}

// ****************************
//
void    FBOProfilingPrototype3::UnlockFrameBuffer  ( SizeType i )
{
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID[ i ] );
    BVGL::bvglUnmapBuffer( GL_PIXEL_PACK_BUFFER );
    BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
}

// ****************************
//
MemoryChunkConstPtr FBOProfilingPrototype3::ReadColor( )
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
void	FBOProfilingPrototype3::AddRenderBufferStorage( SizeType i )
{
	BVGL::bvglBindRenderbuffer( GL_RENDERBUFFER, m_renderBuffersIds[ i ] );
	BVGL::bvglRenderbufferStorage(	GL_RENDERBUFFER,
									GL_RGBA,
									m_width,
									m_height );

	//auto tx = new Texture2DImpl( TextureFormat::F_A8R8G8B8, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC );
	//std::vector< MemoryChunkConstPtr > v;
	//v.push_back( MemoryChunk::Create( m_width * m_height * 4 ) );
	//tx->SetRawData( v, TextureFormat::F_A8R8G8B8, m_width, m_height );
 //   assert( !m_renderer->IsRegistered( tx ) );

 //   PdrTexture2D * pdrTx = PdrTexture2D::Create( tx );
 //   m_renderer->RegisterTexture2D( tx, pdrTx );

 //   //m_drawBuff[ i ] = GL_COLOR_ATTACHMENT0 + i;

 //   BVGL::bvglBindTexture( GL_TEXTURE_2D, pdrTx->GetTextureID() );
 //       
 //   //FIXME: no mipmaps here
 //   //FIXME: only NEAREST filters used here - should be just fine, but some implementations use linear filtering for some reasons here
 //   BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
 //   BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

 //   //BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pdrTx->GetTextureID(), 0 );
}

// **************************
//
IBasicLogicUnqPtr    FBOProfilingPrototype3::Create  ( Renderer * renderer, audio::AudioRenderer * )
{
	return IBasicLogicUnqPtr( new FBOProfilingPrototype3( renderer ) );
}

} // bv
