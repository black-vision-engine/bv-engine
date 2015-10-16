#include "TestLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

namespace bv {

// ****************************
//
bool TestLogic::ms_appInitialized = BasicWindowApp::RegisterInitializer( &TestLogic::Create, "TEST", 800, 600 );

// ****************************
//
#ifdef _DEBUG

bool TestLogic::ms_debugConsole = BasicWindowApp::RegisterConsoleInitializer();

#endif

// ****************************
//
				TestLogic::TestLogic			( Renderer * renderer )
	: m_renderer( renderer )
{
	m_scene = std::make_shared< TestScene >( renderer );
}

// ****************************
//
				TestLogic::~TestLogic			()
{
}

// ****************************
//
void			TestLogic::Initialize			()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// ****************************
//
void			TestLogic::Render				()
{
    m_renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    m_renderer->ClearBuffers();

	Draw( m_scene->GetScene()->GetEngineSceneRoot() );
}

// ****************************
//
void			TestLogic::Update				( TimeType t )
{
	{ t; }
}

// ****************************
//
void			TestLogic::Key					( unsigned char c )
{
	{ c; }
}

// ****************************
//
void			TestLogic::Resize				( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// ****************************
//
void			TestLogic::Draw					( SceneNode * node )
{
	if ( node->IsVisible() )
    {
		m_renderer->Draw( static_cast< bv::RenderableEntity * >( node->GetTransformable() ) );
		for ( unsigned int i = 0; i < (unsigned int) node->NumChildNodes(); i++ )
		{
			Draw( node->GetChild( i ) ); 
		}
	}
}

// **************************
//
IBasicLogicUnqPtr    TestLogic::Create  ( Renderer * renderer )
{
	return IBasicLogicUnqPtr( new TestLogic( renderer ) );
}

} // bv
