#include "FBOProfilingPrototype0.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "LibImage.h"

// FIXME: REMOVE
#include "Tools/HRTimer.h"
#include <random>
#include <iostream>
#include <iomanip>

namespace bv {

namespace {

// **************************
//- Mem transfer tester
void singleTest( char * data, char * res, unsigned int numAccesses, unsigned int entrySize, unsigned int entryTries, std::uniform_int_distribution<> & dis, std::mt19937 & gen )
{
    for( unsigned int i = 0; i < numAccesses; ++i )
    {
        unsigned int offset = dis(gen);
        // std::cout << "    Pass: " << i << ", offset: " << std::setw(2) << offset << std::endl;

        for( unsigned int j = 0; j < entryTries; ++j )
        {
            memcpy(&res[ ( i * entryTries + j ) * entrySize ], &data[ ( offset * entryTries + j ) * entrySize ], entrySize );
        }
    }
}

// **************************
//- Mem transfer tester
void testMemTransferSpeed( unsigned int dataSize, unsigned int numAccesses, unsigned int entrySize, unsigned int entryTries, unsigned int iterations )
{
    std::cout << "Calling test DS: " << dataSize / 1024 / 1024 << "MB, numAcs: " << numAccesses << ", entrSz: " << entrySize << ", entrTr: " << entryTries << ", iteras: " << iterations << std::endl;

    std::cout << "Allocating " << dataSize / 1024 / 1024 << "MB of memory" << std::endl;

    char * data = new char[ dataSize ];
    char * res = new char[ numAccesses * entrySize * entryTries ];

    std::cout << "Initializing memory" << std::endl;

    for( unsigned int i = 0; i < dataSize; ++i )
    {
        data[i] = i % 128;
    }

    unsigned int maxElts = dataSize / ( entrySize * entryTries );

    std::cout << "Initializing rnd generator for interval [0, " << maxElts - 1 << "]" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, maxElts - 1);

    HighResolutionTimer timer;
    auto start = timer.CurElapsed();
    auto intermediate = start;

    for( unsigned int i = 0; i < iterations; ++i )
    {
        // std::cout << "Single mem pass transfer: " << i << std::endl;

        singleTest( data, res, numAccesses, entrySize, entryTries, dis, gen );
    
        double timestamp = timer.CurElapsed();
        double intElapsed = timestamp - intermediate;
        intermediate = timestamp;

        double transferRate = double( entrySize * entryTries * numAccesses ) / ( intElapsed * 1024.f );
        { transferRate; }

        // std::cout << "\nCURRENT TRANSFER: Transferring " << float(entrySize * entryTries * numAccesses) / 1024.f << "KB took " << intElapsed << " secs" << std::endl;
        // std::cout << "CURRENT TRANSFER: " << transferRate << " KB/s" << std::endl;
    }

    auto stop = timer.CurElapsed();

    double transferRate = float( iterations * entrySize * entryTries * numAccesses ) / ( (stop - start) * 1024.f * 1024.f );

    std::cout << "\nTOTAL TRANSFER: Transferring " << float(iterations * entrySize * entryTries * numAccesses) / 1024.f / 1024.f << " MB took " << (stop - start) << " secs" << std::endl;
    std::cout << "TOTAL TRANSFER: " << transferRate << " MB/s" << std::endl;

    delete[] data;
}

} // anonymous 

// FIXME: end of remove

// **************************
//
FBOProfilingPrototype0::FBOProfilingPrototype0    ( Renderer * renderer )
	: m_renderer( renderer )
	, m_rct( 1.f, 1.f )	
	, m_paused( false )
	, m_width( 800 )
	, m_height( 600 )
	, m_enableOffscreenRender( false )
{
    // testMemTransferSpeed( 1073739904, 64, 64, 2, 100 );

	if( !PrepareReadBackBuffers() )
    {
        exit( 1 );
    }

    if( !PrepareShader() )
    {
        exit( 1 );
    }
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

		auto col = ReadColor();
	}
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
	auto buffer = MemoryChunk::Create( Texture2D::RawFrameSize( TextureFormat::F_A8R8G8B8, m_width, m_height ) );

	auto outputTex = Texture2DCache::CreateTexture( TextureFormat::F_A8R8G8B8, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC, buffer );

	memcpy( buffer->GetWritable(), data, outputTex->RawFrameSize() );

	UnlockFrameBuffer( 0 );

	Disable();

	image::SaveBMPImage( "test.bmp", buffer, m_width, m_height, 32 );

	return buffer;
}

// ****************************
//
void	FBOProfilingPrototype0::AddColorAttachments()
{
	auto tx = std::make_shared< Texture2DImpl >( TextureFormat::F_A8R8G8B8, m_width, m_height, DataBuffer::Semantic::S_TEXTURE_STATIC );
	std::vector< MemoryChunkConstPtr > v;
	v.push_back( MemoryChunk::Create( m_width * m_height * 4 ) );
	tx->SetRawData( v, TextureFormat::F_A8R8G8B8, m_width, m_height );
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
