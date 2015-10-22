#include "DrawingPrototype.h"

#include "Prototypes/DrawingCommands/Common/DrawingMode.h"

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
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );

	DrawingTestScene::Initialize();
	DrawingMode::Initialize();

	m_mode = DrawingMode::GetMode( '0' );
	m_scene = DrawingTestScene::GetNextScene(); 
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
	if( c == 'n' )
	{
		delete m_scene;
		m_scene = DrawingTestScene::GetNextScene(); 
		printf( "Drawing scene: %s\n", m_scene->GetName() );
	} 
	else if( c > 40 && c < 65 ) {
		delete m_mode;
		m_mode = DrawingMode::GetMode( c ); 
		printf( "Drawing mode: %s\n", m_mode->GetName() );
	}
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
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	m_scene->Bind();
	m_mode->Render( m_scene->GetDrawingModeData() );
}

// **************************
//
IBasicLogicUnqPtr    DrawingPrototype::Create  ( Renderer * renderer )
{
	return IBasicLogicUnqPtr( new DrawingPrototype( renderer ) );
}

} // bv
