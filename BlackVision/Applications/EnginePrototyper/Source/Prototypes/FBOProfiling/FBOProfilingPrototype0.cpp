#include "FBOProfilingPrototype0.h"

#include "Engine/Graphics/Renderers/Renderer.h"

namespace bv {

// **************************
//
FBOProfilingPrototype0::FBOProfilingPrototype0    ( Renderer * renderer )
	: m_renderer( renderer )
	, m_rct( 1.f, 1.f )	
	, m_paused( false )
{

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
    if( m_paused )
        return;

    float r = ( cos( t ) + 1.f ) * 0.5f;
    float g = ( sin( t ) + 1.f ) * 0.5f;
    float b = ( cos( t / 3.f ) * sin( t / 2.f ) + 1.f ) * 0.5f;

    m_prog.SetUniform( "color", glm::vec4( r, g, b, 1.f ) );
}

// **************************
//
void    FBOProfilingPrototype0::Render         ()
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_prog.Use();
	m_rct.Render();
}

// **************************
//
void    FBOProfilingPrototype0::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;
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

} // bv
