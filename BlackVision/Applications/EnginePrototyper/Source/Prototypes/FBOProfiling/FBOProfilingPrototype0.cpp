#include "FBOProfilingPrototype0.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/Texture2D.h"
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
FBOProfilingPrototype0::FBOProfilingPrototype0    ( Renderer * renderer )
	: FBOProfilingPrototypeBase( renderer )
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
FBOProfilingPrototype0::~FBOProfilingPrototype0   ()
{
}

// **************************
//
void    FBOProfilingPrototype0::Initialize     ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// **************************
//
void    FBOProfilingPrototype0::Update         ( TimeType t )
{


    float r = ( cos( t ) + 1.f ) * 0.5f;
    float g = ( sin( t ) + 1.f ) * 0.5f;
    float b = ( cos( t / 3.f ) * sin( t / 2.f ) + 1.f ) * 0.5f;

    m_prog.SetUniform( "color", glm::vec4( r, g, b, 1.f ) );
}

// **************************
//
void    FBOProfilingPrototype0::Render         ()
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
		std::cout << "One frame time is: " << stopTime - startTime << std::endl;
	}

	m_currFrame++;
}

// **************************
//
void    FBOProfilingPrototype0::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;

	m_enableOffscreenRender = !m_enableOffscreenRender;
}

// **************************
//
void    FBOProfilingPrototype0::Resize         ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// **************************
//
bool    FBOProfilingPrototype0::PrepareShader  ()
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
void    FBOProfilingPrototype0::Enable()
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID[ 0 ] );
	BVGL::bvglDrawBuffers( ( GLsizei )1, &m_drawBuff );

	//BVGL::bvglGetIntegerv( GL_VIEWPORT, m_prevViewportCoords );
	//BVGL::bvglGetDoublev( GL_DEPTH_RANGE, m_prevDepthRange );

	//BVGL::bvglViewport( 0, 0, m_width, m_height );
	//BVGL::bvglDepthRange( 0.0, 1.0 );
}

// **************************
//
void    FBOProfilingPrototype0::Disable()
{
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );

    //BVGL::bvglViewport( m_prevViewportCoords[ 0 ], m_prevViewportCoords[ 1 ], m_prevViewportCoords[ 2 ], m_prevViewportCoords[ 3 ] );
    //BVGL::bvglDepthRange( m_prevDepthRange[ 0 ], m_prevDepthRange[ 1 ] );
}

// **************************
//
bool    FBOProfilingPrototype0::PrepareReadBackBuffers()
{
	BVGL::bvglGenBuffers( 1, &m_pboID );
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID );
    BVGL::bvglBufferData( GL_PIXEL_PACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_READ );
    BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );

	BVGL::bvglGenFramebuffers( 1, m_fboID );
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID[ 0 ] );

	AddColorAttachments();

	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );

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
void *   FBOProfilingPrototype0::LockFrameBuffer( SizeType i )
{
	BVGL::bvglReadBuffer( m_fboID[ i ] );

	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID );
	BVGL::bvglReadPixels( 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	//BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID );
	void * memory = BVGL::bvglMapBuffer( GL_PIXEL_PACK_BUFFER, GL_READ_WRITE );

	//BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );

	return memory;
}

// ****************************
//
void    FBOProfilingPrototype0::UnlockFrameBuffer  ( SizeType i )
{
	{ i; }
	BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, m_pboID );
    BVGL::bvglUnmapBuffer( GL_PIXEL_PACK_BUFFER );
    BVGL::bvglBindBuffer( GL_PIXEL_PACK_BUFFER, 0 );
}

// ****************************
//
MemoryChunkConstPtr FBOProfilingPrototype0::ReadColor()
{
	
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID[ 0 ] );

	auto data = LockFrameBuffer( 0 );
	{data;}
	//memcpy( m_buffer->GetWritable(), data, m_rawFrameSize );

	UnlockFrameBuffer( 0 );

	Disable();

	//image::SaveBMPImage( std::to_string( m_currFrame ) + ".bmp", m_buffer, m_width, m_height, 32 );

	return m_buffer;
}

// ****************************
//
void	FBOProfilingPrototype0::AddColorAttachments()
{
	auto tx = std::make_shared< Texture2D >( TextureFormat::F_A8R8G8B8, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC );
	std::vector< MemoryChunkConstPtr > v;
	v.push_back( MemoryChunk::Create( m_width * m_height * 4 ) );
	tx->SetData( v );
    assert( !m_renderer->IsRegistered( tx.get() ) );

    PdrTexture2D * pdrTx = PdrTexture2D::Create( tx.get() );
    m_renderer->RegisterTexture2D( tx.get(), pdrTx );

    m_texId = pdrTx->GetTextureID();
    m_drawBuff = GL_COLOR_ATTACHMENT0;

    BVGL::bvglBindTexture( GL_TEXTURE_2D, pdrTx->GetTextureID() );
        
    //FIXME: no mipmaps here
    //FIXME: only NEAREST filters used here - should be just fine, but some implementations use linear filtering for some reasons here
    BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, m_drawBuff, GL_TEXTURE_2D, m_texId, 0 );
}


// ****************************
//
void	FBOProfilingPrototype0::EnableFrameBuffer	()
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID[ 0 ] );
    BVGL::bvglDrawBuffers( ( GLsizei )1, &m_drawBuff );

    BVGL::bvglGetIntegerv( GL_VIEWPORT, m_prevViewportCoords );
    BVGL::bvglGetDoublev( GL_DEPTH_RANGE, m_prevDepthRange );

    BVGL::bvglViewport( 0, 0, m_width, m_height );
    BVGL::bvglDepthRange( 0.0, 1.0 );
}

// ****************************
//
void	FBOProfilingPrototype0::DisableFrameBuffer	()
{
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );

    BVGL::bvglViewport( m_prevViewportCoords[ 0 ], m_prevViewportCoords[ 1 ], m_prevViewportCoords[ 2 ], m_prevViewportCoords[ 0 ] );
    BVGL::bvglDepthRange( m_prevDepthRange[ 0 ], m_prevDepthRange[ 1 ] );
}

} // bv
