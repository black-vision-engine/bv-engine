#include "TestLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Scenes/TestSceneUtils.h"

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
				TestLogic::TestLogic			( Renderer * renderer, audio::AudioRenderer * audioRenderer )
	: m_renderer( renderer )
{
    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( TestSceneUtils::DefaultBVPluginDescriptors() );

	m_project = BVProject::Create( m_renderer, audioRenderer, TestSceneUtils::GetConfig() );

	m_scene = std::make_shared< TestScene >( m_project.get(), renderer );
	
	auto cam = m_renderer->GetCamera();
	cam->SetPerspective( 90.0f, 800, 600, 0.1f, 1000.0f );
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
}

// ****************************
//
void			TestLogic::Render				()
{
    m_renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
    m_renderer->ClearBuffers();

	Draw( m_project->GetEngineSceneRoot() );
	
}

// ****************************
//
void			TestLogic::Update				( TimeType t )
{
	m_scene->TestEditor( t );

	m_project->Update( t );

	m_time = t;
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
	m_renderer->Resize( w, h );
}

// ****************************
//
void			TestLogic::Draw					( SceneNode * node )
{
	if ( node && node->IsVisible() )
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
IBasicLogicUnqPtr    TestLogic::Create  ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
	return IBasicLogicUnqPtr( new TestLogic( renderer, audioRenderer ) );
}

} // bv
