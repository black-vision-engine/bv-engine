#include "SimpleVAOPrototype0.h"

namespace bv {

// **************************
//
SimpleVAOPrototype0::SimpleVAOPrototype0    ()
    : m_rct( 1.f, 1.f )
    , m_paused( false )
{
    if( !PrepareShader() )
    {
        exit( 1 );
    }
}

// **************************
//
SimpleVAOPrototype0::~SimpleVAOPrototype0   ()
{
}

// **************************
//
void    SimpleVAOPrototype0::Initialize     ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// **************************
//
void    SimpleVAOPrototype0::Update         ( TimeType t )
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
void    SimpleVAOPrototype0::Render         ()
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_prog.Use();
    m_rct.Render();
}

// **************************
//
void    SimpleVAOPrototype0::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;
}

// **************************
//
void    SimpleVAOPrototype0::Resize         ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// **************************
//
bool    SimpleVAOPrototype0::PrepareShader  ()
{
    std::string shadersRoot = config::PROTOTYPES_SHADERS_ROOT + "VAOPrototype0/";

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
