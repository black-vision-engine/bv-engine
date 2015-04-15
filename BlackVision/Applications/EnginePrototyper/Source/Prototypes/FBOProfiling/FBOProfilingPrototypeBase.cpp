#include "FBOProfilingPrototypeBase.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/Texture2DImpl.h"
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
FBOProfilingPrototypeBase::FBOProfilingPrototypeBase    ( Renderer * renderer )
	: m_renderer( renderer )
	, m_rct( 1.f, 1.f )
	, m_paused( false )
	, m_width( 1920 )
	, m_height( 1080 )
	, m_enableOffscreenRender( false )
	, m_currFrame( 0 )
{
    //CacheMissBenchmark b( 1073739904, 64, 64, 2 );
    //b.RunBenchmark( 100 );

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
FBOProfilingPrototypeBase::~FBOProfilingPrototypeBase   ()
{
}

// **************************
//
void    FBOProfilingPrototypeBase::Initialize     ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// **************************
//
void    FBOProfilingPrototypeBase::Update         ( TimeType t )
{
    float r = ( cos( t ) + 1.f ) * 0.5f;
    float g = ( sin( t ) + 1.f ) * 0.5f;
    float b = ( cos( t / 3.f ) * sin( t / 2.f ) + 1.f ) * 0.5f;

    m_prog.SetUniform( "color", glm::vec4( r, g, b, 1.f ) );
}

// **************************
//
void    FBOProfilingPrototypeBase::Render         ()
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
}

// **************************
//
void    FBOProfilingPrototypeBase::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;

	m_enableOffscreenRender = !m_enableOffscreenRender;
}

// **************************
//
void    FBOProfilingPrototypeBase::Resize         ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// **************************
//
bool    FBOProfilingPrototypeBase::PrepareShader  ()
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

} // bv
