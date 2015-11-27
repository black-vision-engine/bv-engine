#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/BVSceneEditor.h"

#include "Tools/SimpleTimer.h"
#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Rendering/Logic/RenderLogic.h"
#include "ModelInteractionEvents.h"

#include "Widgets/Crawler/CrawlerEvents.h"

#include "System/Env.h"
#include "BVConfig.h"
#include "ProjectManager.h"
#include "Serialization/XML/XMLSerializer.h"

#include "MockScenes.h"
#include "DefaultPlugins.h"
#include "LibEffect.h"

//FIXME: remove
#include "TestAI/TestGlobalEffectKeyboardHandler.h"
#include "TestAI/TestEditorsKeyboardHandler.h"
#include "TestAI/TestVideoStreamDecoderKeyboardHandler.h"

#include "testai/TestAIManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "BVGL.h"
//FIXME: end of remove

#include"StatsFormatters.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "EndUserAPI/EventHandlers/RemoteEventsHandlers.h"
#include "EndUserAPI/JsonCommandsListener/JsonCommandsListener.h"

//pablito
#define XML
#include "ConfigManager.h"


namespace bv
{
extern HighResolutionTimer GTimer;
namespace
{
    //FIXME: temporary
    TransformSetEventPtr  GTransformSetEvent;

    void GownoWFormieKebaba( TimeType t, BVAppLogic * logic )
    {
        { logic; }
        //DETERMINSTIC TIME INTERVALS
        //static TimeType tt = TimeType( 0.0 );
        //tt += TimeType( 0.001 );

        //TEST AI
        //static auto ai = TestAIManager::Instance().GetAIPreset( 1 );
        //static auto ai = TestAIManager::Instance().GetAIPreset( 5, logic );
        //static auto ai = TestAIManager::Instance().GetAIPreset( 4, logic );
        
        //static auto ai = TestAIManager::Instance().GetAIPreset( 2, logic->GetModelScene()->GetSceneRoot() );
        //ai->EvalAt( t );

        //Override alpha test events
        //static auto ai = TestAIManager::Instance().GetAIPreset( 3, logic->GetModelScene()->GetSceneRoot() );

        //Override node mask test events
        //static auto ai = TestAIManager::Instance().GetAIPreset( 6, logic->GetModelScene()->GetSceneRoot() );
        //ai->EvalAt( t );

        //PRE GOWNO
        float tx = float( sin( t ) );
        glm::vec3 kebab( tx, 0.f, 0.f );

        //gowno
        GTransformSetEvent->SetTranslation( kebab );
    
        GetDefaultEventManager().QueueEvent( GTransformSetEvent );
    }

    KeyPressedEventPtr  GKeyPressedEvent;

    void KeyPressedSendEvent( unsigned char c )
    {
        GKeyPressedEvent->SetChar( c );
        GetDefaultEventManager().QueueEvent( GKeyPressedEvent );
    }
}

//
BVAppLogic::BVAppLogic              ( Renderer * renderer )
    : m_bvScene( BVScene::Create( renderer ) )
    , m_pluginsManager( nullptr )
    , m_renderer( nullptr )
    , m_renderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
	, m_solution( model::TimelineManager::GetInstance() )
{
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
    GKeyPressedEvent = KeyPressedEventPtr( new KeyPressedEvent() );
    GTimer.StartTimer();

    m_renderer = renderer;
    m_renderLogic = new RenderLogic();
    m_remoteHandlers = new RemoteEventsHandlers;
    m_remoteController = new JsonCommandsListener;
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
    delete m_renderLogic;
    
    delete m_remoteHandlers;
    delete m_kbdHandler;

    delete m_remoteController;
}

// *********************************
//
void BVAppLogic::Initialize         ()
{
    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
    m_pluginsManager = &model::PluginsManager::DefaultInstance();

    bv::effect::InitializeLibEffect( m_renderer );

    InitializeKbdHandler();
    InitializeRemoteCommunication();
}

// *********************************
//
void BVAppLogic::LoadScenes( const PathVec & pathVec )
{
	m_bvScene->GetSceneEditor()->RemoveAllScenes();

    for( auto p : pathVec )
    {
		auto scene = SceneDescriptor::LoadScene( ProjectManager::GetInstance()->ToAbsPath( p ) );
		m_bvScene->GetSceneEditor()->AddScene( scene );
    }

    InitCamera( DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight() );
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    //auto te = m_timelineManager->CreateDefaultTimeline( "", 10.f, TimelineWrapMethod::TWM_MIRROR, TimelineWrapMethod::TWM_MIRROR );
    //te->Play();
    //m_globalTimeline->AddChild( te );

    if( !ConfigManager::GetBool( "Debug/LoadSceneFromEnv" ) )
    {
        if( ConfigManager::GetBool( "Debug/LoadSolution" ) )
        {
            //m_solution.SetTimeline(m_timelineManager);
            m_solution.LoadSolution( ConfigManager::GetString("solution") );

            auto root = m_solution.GetRoot();
			auto scene = SceneModel::Create( "root", root, m_renderer->GetCamera() );

			m_bvScene->GetSceneEditor()->AddScene( scene );

            //if(ConfigManager::GetBool("hm"))
            //root->AddChildToModelOnly(TestScenesFactory::NewModelTestScene( m_pluginsManager, m_timelineManager, m_globalTimeline ));
        }
        else
        {
            auto pm = ProjectManager::GetInstance();

            auto projectName = ConfigManager::GetString( "default_project_name" );
            
            if( !projectName.empty() )
            {
                auto projectScenesNames = pm->ListScenesNames( projectName );

                if( !projectScenesNames.empty() )
                {
                    LoadScenes( projectScenesNames );
                }
            }
        }
    }
    else
    {
		m_bvScene->GetSceneEditor()->AddScene( TestScenesFactory::CreateSceneFromEnv( GetEnvScene(), m_renderer->GetCamera(), m_pluginsManager ) );
    }
}

// *********************************
//
void BVAppLogic::InitCamera         ( unsigned int w, unsigned int h )
{
	auto cam = new Camera( DefaultConfig.IsCameraPerspactive() );

    cam->SetFrame( DefaultConfig.CameraPosition(), DefaultConfig.CameraDirection(), DefaultConfig.CameraUp() );
    
    if( cam->IsPerspective() )
    {
        cam->SetPerspective( DefaultConfig.FOV(), w, h, DefaultConfig.NearClippingPlane(), DefaultConfig.FarClippingPlane() );
    }
    else
    {
        cam->SetViewportSize( w, h );
    }

	//FIXME: nobody owns camera right now.. so it will be deleted here instead of scene model
	if( m_renderer->GetCamera() )
	{
		delete m_renderer->GetCamera();
	}

    m_renderer->SetCamera( cam );
    m_renderLogic->SetCamera( cam );

    //FIXME: read from configuration file and change the camera appropriately when current resoultion changes
}

// *********************************
//
void BVAppLogic::SetStartTime       ( unsigned long millis )
{
    m_renderMode.SetStartTime( millis );
	m_bvScene->SetStartTime( millis );
}

// *********************************
//
void BVAppLogic::OnUpdate           ( unsigned int millis, Renderer * renderer )
{
    HPROFILER_FUNCTION( "BVAppLogic::OnUpdate", PROFILER_THREAD1 );

    TimeType time = m_renderMode.StartFrame( millis );
    UpdateFrame( time, renderer );
}

// ***********************
//
void BVAppLogic::UpdateFrame     ( TimeType time, Renderer * renderer )
{
    assert( m_state != BVAppState::BVS_INVALID );
    if( m_state == BVAppState::BVS_RUNNING )
    {
        FRAME_STATS_FRAME();
        FRAME_STATS_SECTION( DefaultConfig.FrameStatsSection() );

        GownoWFormieKebaba( time, this );

        {
            FRAME_STATS_SECTION( "Update" );
            HPROFILER_SECTION( "update total", PROFILER_THREAD1 );

            m_bvScene->Update( time );
        }

        m_remoteHandlers->UpdateHM();

        {
            HPROFILER_SECTION( "Render", PROFILER_THREAD1 );			
            
            {
                FRAME_STATS_SECTION( "Video input" );
		        RefreshVideoInputScene();
            }

            {
                FRAME_STATS_SECTION( "Render" );
                m_renderLogic->RenderFrame( renderer, m_bvScene->GetEngineSceneRoot() );
            }
        }
    }

    GTimer.StartTimer();
}


// *********************************
//
void BVAppLogic::RefreshVideoInputScene()
{
    if(ConfigManager::GetBool("Debug/UseVideoInputFeeding") && m_videoCardManager->IsEnabled())
    {
        if(m_videoCardManager->CheckIfNewFrameArrived(0,"A"))
        {
            BB::AssetManager::VideoInput->RefreshData(m_videoCardManager->GetCaptureBufferForShaderProccessing(0,"A"));
        }
        else
        {
            m_videoCardManager->UnblockCaptureQueue(0,"A");
        }
    }
}

// *********************************
//
void BVAppLogic::OnKey           ( unsigned char c )
{
    m_kbdHandler->HandleKey( c, this );
}

// *********************************
//
void BVAppLogic::ChangeState     ( BVAppState state )
{
    if( state == BVAppState::BVS_INITALIZING )
    {
        assert( m_state != BVAppState::BVS_RUNNING );
    }

    m_state = state;
}

// *********************************
//
void BVAppLogic::ShutDown           ()
{
    //TODO: any required deinitialization
}

//pablito:
void	BVAppLogic::SetVideoCardManager(bv::videocards::VideoCardManager* videoCardManager)
{
        m_videoCardManager = videoCardManager;
        m_renderLogic->SetVideoCardManager(videoCardManager,m_renderer);
}

// *********************************
//
void    BVAppLogic::PostFrameLogic   ( const SimpleTimer & timer, unsigned int millis )
{
    if( m_statsCalculator.WasSampledMaxVal( DefaultConfig.FrameStatsSection() ) )
    {
        unsigned int frame = m_statsCalculator.CurFrame() - 1;

#ifndef HIDE_PROFILE_STATS
        FrameStatsFormatter::PrintFrameStatsToConsole( frame, m_statsCalculator, "LONGEST FRAME SO FAR", 10 );
        HPROFILER_SET_FORCED_DISPLAY();
#endif
    }

    if( m_statsCalculator.CurFrame() == DefaultConfig.MAVWarmupRounds() * m_statsCalculator.WindowSize() || m_statsCalculator.CurFrame() % DefaultConfig.StatsRecalcFramesDelta() == 0 )
    {
        m_statsCalculator.RecalculateStats();
#ifndef HIDE_PROFILE_STATS
        FrameStatsFormatter::PrintToConsole( m_statsCalculator );
#endif
    }

    unsigned long frameMillis = timer.ElapsedMillis() - millis;
    if( frameMillis < DefaultConfig.FrameTimeMillis() )
    {
        Sleep( DefaultConfig.FrameTimeMillis() - frameMillis );
        printf( "Sleeping: %d\n", DefaultConfig.FrameTimeMillis() - frameMillis );
    }
}

// *********************************
//
const FrameStatsCalculator &     BVAppLogic::FrameStats () const
{
    return m_statsCalculator;
}

// *********************************
//
void                            BVAppLogic::ResetScene      ()
{
    UpdatersManager::Get().RemoveAllUpdaters();
    m_bvScene = nullptr;
}

// *********************************
//
void                            BVAppLogic::ReloadScene     ()
{
    ResetScene();
    LoadScene();
}

//pablito
// *********************************
//
void            BVAppLogic::GrabCurrentFrame(  const std::string & path )
{
    m_grabFramePath = path;
}

// *********************************
//FIXME: unsafe - consider returning const variant of this class (IParameters * without const should be accessible anyway)
BVScenePtr                  BVAppLogic::GetBVScene              ()
{
    return m_bvScene;
}

// *********************************
//
const model::PluginsManager *   BVAppLogic::GetPluginsManager   () const
{
    return m_pluginsManager;
}

// *********************************
//
void                            BVAppLogic::InitializeKbdHandler()
{
    auto envScene = GetEnvScene();

    if ( envScene == "GLOBAL_EFFECT_05" )
    {
        m_kbdHandler = new TestGlobalEfectKeyboardHandler();
    }
    else if( envScene == "SERIALIZED_TEST" )
    {
        m_kbdHandler = new TestEditorsKeyboardHandler();
    }
    else if( envScene == "VIDEO_STREAM_TEST_SCENE" )
    {
        m_kbdHandler = new TestVideoStreamDecoderKeyboardHandler();
    }
    else if( envScene == "LIGHT_SCATTERING_EFFECT" )
    {
        m_kbdHandler = new TestEditorsKeyboardHandler();
    }
    else
    {
        m_kbdHandler = new TestKeyboardHandler();
    }
}

// *********************************
//
void                            BVAppLogic::InitializeRemoteCommunication()
{
    m_remoteHandlers->InitializeHandlers( this );

    unsigned int editorPort = ConfigManager::GetInt( "Network/SocketServer/Port" );
    m_remoteController->InitializeServer( editorPort );
}

// *********************************
//
std::string                     BVAppLogic::GetEnvScene()
{
    auto s = ConfigManager::GetString( "Debug/SceneFromEnvName" );
    if( s != "" )
        return s;
    else
        return Env::GetVar( DefaultConfig.DefaultSceneEnvVarName() );
}

//// *********************************
////
//void BlackVisionApp::ReadBackFrameBuffer ()
//{
//    if ( !DefaultConfig.ReadbackFlag() )
//    {
//        return;
//    }
//
//    GframeRenderedEvent->SetResolution( m_Width, m_Height );
//    GEventManager->TriggerEvent( GframeRenderedEvent );
//}

//SOME code for hands-on profiling
        //static unsigned int frame = 0;
        //    static TimeType tt = TimeType( 0 );
        //    double elasp = GTimer.CurElapsed();
        //    if( elasp > 0.01 )
        //    {
        //        printf ( "%d %1.4f s -> g: %1.4f u: %1.4f r: %1.4f \n", frame, 1000. * elasp, 1000. * ( ge - gs ), 1000. * ( ue - us ), 1000. * ( GTimer.Re() ) );
        //        t = TimeType( 0 );
        //    }
   
        //    tt += t;
        //    frame++;

} //bv
