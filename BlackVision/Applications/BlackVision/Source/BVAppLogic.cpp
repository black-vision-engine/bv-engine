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
#include "TestAI/TestRemoteEventsKeyboardHandler.h"
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
    : m_startTime( 0 )
    , m_timelineManager( std::make_shared < model::TimelineManager >() )
    , m_bvScene( nullptr )
    , m_pluginsManager( nullptr )
    , m_renderer( nullptr )
    , m_renderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
    , m_globalTimeline( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) )
    , m_solution( GetTimelineManager().get() ) //pablito
{
    model::TimelineManager::SetInstance( GetTimelineManager().get() );
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
    GetTimelineManager()->RegisterRootTimeline( m_globalTimeline );

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
    m_pluginsManager = &model::PluginsManager::DefaultInstance();

    bv::effect::InitializeLibEffect( m_renderer );

    InitializeKbdHandler();
    m_remoteHandlers->InitializeHandlers( this );
    m_remoteController->InitializeRemoteLog( "10.0.0.101", 28777, SeverityLevel::info );
    m_remoteController->InitializeServer( 11101 );
}

// *********************************
//
void BVAppLogic::LoadScenes( const PathVec & pathVec )
{
    model::SceneModelVec sceneModelVec;

    for( auto p : pathVec )
    {
        auto scene = SceneDescriptor::LoadScene( ProjectManager::GetInstance()->ToAbsPath( p ), GetTimelineManager().get() );
        sceneModelVec.push_back( scene );
    }

    Camera * cam = nullptr;

    if( m_bvScene )
    {
        cam = m_bvScene->GetCamera();
    }
    else
    {
        cam = new Camera( DefaultConfig.IsCameraPerspactive() );
    }

    m_bvScene    = BVScene::Create( sceneModelVec, cam, m_globalTimeline, m_renderer );
    assert( m_bvScene );
    InitializeScenesTimelines();

    InitCamera( DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight() );
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    //auto te = m_timelineManager->CreateDefaultTimeline( "", 10.f, TimelineWrapMethod::TWM_MIRROR, TimelineWrapMethod::TWM_MIRROR );
    //te->Play();
    //m_globalTimeline->AddChild( te );
    auto te = m_globalTimeline;

    m_timelineManager->RegisterRootTimeline( te );
    
    if( !ConfigManager::GetBool( "Debug/LoadSceneFromEnv" ) )
    {
        if( ConfigManager::GetBool( "Debug/LoadSolution" ) )
        {
            //m_solution.SetTimeline(m_timelineManager);
            m_solution.LoadSolution( ConfigManager::GetString("solution") );
            auto root = m_solution.GetRoot();
            m_bvScene    = BVScene::Create( SceneModel::Create( "root", GetTimelineManager(), root ), new Camera( DefaultConfig.IsCameraPerspactive() ), te, m_renderer );
            InitializeScenesTimelines();
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
        auto scene = TestScenesFactory::CreateSceneFromEnv( GetEnvScene(), m_pluginsManager, GetTimelineManager(), m_globalTimeline );
        m_bvScene = BVScene::Create( scene, new Camera( DefaultConfig.IsCameraPerspactive() ), te, m_renderer );
        if( GetEnvScene() == "SERIALIZED_TEST" ) // FIXME: nasty hack
            InitializeScenesTimelines();
    }

    if( !m_bvScene )
    {
        m_bvScene = BVScene::Create( model::SceneModelVec(), new Camera( DefaultConfig.IsCameraPerspactive() ), te, m_renderer );
        InitializeScenesTimelines();
    }
}

// *********************************
//
void BVAppLogic::InitCamera         ( unsigned int w, unsigned int h )
{
    Camera * cam = m_bvScene->GetCamera();

    cam->SetFrame( DefaultConfig.CameraPosition(), DefaultConfig.CameraDirection(), DefaultConfig.CameraUp() );
    
    if( cam->IsPerspective() )
    {
        cam->SetPerspective( DefaultConfig.FOV(), w, h, DefaultConfig.NearClippingPlane(), DefaultConfig.FarClippingPlane() );
    }
    else
    {
        cam->SetViewportSize( w, h );
    }

    m_renderer->SetCamera( cam );
    m_renderLogic->SetCamera( cam );

    //FIXME: read from configuration file and change the camera appropriately when current resoultion changes
}

// *********************************
//
void BVAppLogic::SetStartTime       ( unsigned long millis )
{
    m_startTime = millis;
    m_globalTimeline->SetTimeOffset( -TimeType( millis ) * TimeType( 0.001 ) );
}

// *********************************
//
void BVAppLogic::OnUpdate           ( unsigned int millis, Renderer * renderer )
{
    HPROFILER_FUNCTION( "BVAppLogic::OnUpdate", PROFILER_THREAD1 );

    assert( m_state != BVAppState::BVS_INVALID );
    if( m_state == BVAppState::BVS_RUNNING )
    {
        FRAME_STATS_FRAME();
        FRAME_STATS_SECTION( DefaultConfig.FrameStatsSection() );

        //FIXME: debug timer - don't get fooled
        //float t = float(frame) * 0.1f; ///10 fps

        TimeType t = TimeType( millis ) * TimeType( 0.001 );
        GownoWFormieKebaba( t, this );

        {
            FRAME_STATS_SECTION( "Update" );
            HPROFILER_SECTION( "update total", PROFILER_THREAD1 );

            m_globalTimeline->SetGlobalTime( t );
            m_bvScene->Update( t );
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
            
            {
                FRAME_STATS_SECTION( "VideoCard copy buffer" );
                m_renderLogic->FrameRendered( renderer );
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
    m_globalTimeline = model::OffsetTimeEvaluatorPtr( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) );
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
//
model::TimelineManagerPtr	BVAppLogic::GetTimelineManager      ()
{
    return m_timelineManager;
}

// *********************************
//
model::OffsetTimeEvaluatorPtr   BVAppLogic::GetGlobalTimeline   ()
{
    return m_globalTimeline;
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
    else if( envScene == "REMOTE_EVENTS_TEST_SCENE" )
    {
        //m_kbdHandler = new TestEditorsKeyboardHandler();
        m_kbdHandler = new TestRemoteEventsKeyboardHandler();
    }
    else
    {
        m_kbdHandler = new TestKeyboardHandler();
    }
}

// *********************************
//
void                            BVAppLogic::InitializeScenesTimelines()
{
    for( auto scene : m_bvScene->GetScenes() )
        m_timelineManager->AddTimeline( scene->m_pTimelineManager->GetRootTimeline() );
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
