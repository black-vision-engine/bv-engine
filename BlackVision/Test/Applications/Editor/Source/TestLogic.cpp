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
				TestLogic::TestLogic			( Renderer * renderer )
	: m_renderer( renderer )
    , m_timelineManager( new model::TimelineManager() )
    , m_globalTimeline( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) )
{
    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( TestSceneUtils::DefaultBVPluginDescriptors() );

    m_timelineManager->RegisterRootTimeline( m_globalTimeline );
	m_scene = std::make_shared< TestScene >( renderer, m_timelineManager, m_globalTimeline );
	
	auto cam = m_renderer->GetCamera();
	cam->SetPerspective( 90.0f, 800, 600, 0.1f, 1000.0f );
}

// ****************************
//
				TestLogic::~TestLogic			()
{
    delete m_timelineManager;
}

// ****************************
//
void			TestLogic::Initialize			()
{
    //m_globalTimeline->SetTimeOffset( TimeType( -2.f ) );
}

// ****************************
//
void			TestLogic::Render				()
{
    m_renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
    m_renderer->ClearBuffers();

	Draw( m_scene->GetScene()->GetEngineSceneRoot() );
	
}

// ****************************
//
void			TestLogic::Update				( TimeType t )
{
	m_scene->TestEditor( t );

    m_globalTimeline->SetGlobalTime( t );
	m_scene->GetScene()->Update( t );

	m_time = t;
}

// ****************************
//
void			TestLogic::Key					( unsigned char c )
{
	{ c; }

	if( c == 'r' )
	{
		m_globalTimeline->SetTimeOffset( -m_time );
		m_scene->Restart();
		Update( m_time );
	}
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
IBasicLogicUnqPtr    TestLogic::Create  ( Renderer * renderer )
{
	return IBasicLogicUnqPtr( new TestLogic( renderer ) );
}

} // bv
