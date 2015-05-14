#include "DrawingPrototype.h"

#include "Prototypes/DrawingCommands/Common/DrawingMode.h"
#include "Common/BVGLDefsExt.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

namespace bv {

// *****************************
//
DrawingPrototype::DrawingPrototype      ( Renderer * renderer )
    : m_mode( nullptr )
	, m_scene( nullptr )
{
	{ renderer; }
}

// *****************************
//
DrawingPrototype::~DrawingPrototype     ()
{
    delete m_mode;
}

// *****************************
//
void    DrawingPrototype::Initialize          ()
{
    BVGLExt::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

    m_mode = DrawingMode::CreateDrawingMode( 0 );
	m_scene = DrawingTestScene::CreateDrawingTestScene( 0 ); 

	PrepareShader();
}

// *****************************
//
void    DrawingPrototype::Update              ( TimeType t )
{
	{ t; }
}

// *****************************
//
void    DrawingPrototype::Key                 ( unsigned char c )
{
	{ c; }
}

// *****************************
//
void    DrawingPrototype::Resize              ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// *****************************
//
void    DrawingPrototype::Render              ()
{
    BVGLExt::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
	m_prog.Use();
	
	m_scene->Bind();
	m_mode->Render( m_scene->GetDrawingModeData() );
}

// **************************
//
bool    DrawingPrototype::PrepareShader		()
{
    std::string shadersRoot = config::PROTOTYPES_SHADERS_ROOT + "DrawingCommands/";

    std::string vsFile = shadersRoot + "default.vert";
    std::string psFile = shadersRoot + "default.frag";

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

    return true;
}

} // bv
