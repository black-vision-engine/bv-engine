#include "stdafxBVApp.h"

#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Audio/AudioRenderer.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Models/ModelState.h"

#include "Tools/SimpleTimer.h"
#include "Tools/Profiler/HerarchicalProfiler.h"
#include "Services/BVServiceProvider.h"

#include "Application/ApplicationContext.h"

// FIXME: nrl - render logic replacement
//#include "Engine/Graphics/Effects/Logic/RenderLogic.h"

#include "Widgets/NodeLogicFactory.h"

#include "System/Env.h"
#include "BVConfig.h"
#include "ProjectManager.h"
#include "VideoCardManager.h"

#include "MockScenes.h"
#include "DefaultPlugins.h"
#include "LibEffect.h"

//FIXME: remove
#include "TestAI/TestGlobalEffectKeyboardHandler.h"
#include "TestAI/TestEditorsKeyboardHandler.h"
#include "TestAI/TestVideoStreamDecoderKeyboardHandler.h"
#include "TestAI/TestInnerEvents.h"

#include "TestAI/TestVideoOutputKeyboardHandler.h"
#include "testai/TestAIManager.h"
//FIXME: end of remove

#include "StatsFormatters.h"

#include "UseLoggerBVAppModule.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "EndUserAPI/EventHandlers/RemoteEventsHandlers.h"
#include "EndUserAPI/JsonCommandsListener/JsonCommandsListener.h"

#include "Initialization/RenderLogicInitializer.h"

#include <thread>
#include <chrono>

//pablito
#define XML

#define HIDE_PROFILE_STATS

namespace bv
{
extern HighResolutionTimer GTimer;

// *********************************
//
BVAppLogic::BVAppLogic              ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
    : m_bvProject( BVProject::Create( renderer, audioRenderer ) )
    , m_pluginsManager( nullptr )
    , m_renderer( nullptr )
    , m_audioRenderer( nullptr )
    , m_renderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
	, m_gain( 1.f )
    , m_videoCardManager( nullptr )
{
    GTimer.StartTimer();

    m_renderer = renderer;
    m_audioRenderer = audioRenderer;

    // nrl - render logic replacement
    //m_renderLogic = new RenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.ClearColor(), DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput(), DefaultConfig.RenderToSharedMemory(), DefaultConfig.SharedMemoryScaleFactor());
    
    // FIXME: nrl - pass all those arguments in a struct
    // m_renderLogic = new nrl::NRenderLogicImpl( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), 2 ); //, DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput() );
    // FIXME: prepare descriptor here

    m_renderLogic = nrl::RenderLogicInitializer::CreateInstance( DefaultConfig );

    m_remoteHandlers = new RemoteEventsHandlers;
    m_remoteController = new JsonCommandsListener;

    model::ModelState::GetInstance().RegisterBVProject( m_bvProject.get() );

    //m_renderLogic = new FrameRenderLogic();
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
    m_renderMode.Init( m_renderLogic, m_renderer );

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );

    m_pluginsManager = &model::PluginsManager::DefaultInstance();

    bv::effect::InitializeLibEffect( m_renderer );

    SetNodeLogicFactory( new NodeLogicFactory );

    InitializeKbdHandler();
    InitializeRemoteCommunication();
    InitializeCommandsDebugLayer();

    ProjectManager::SetPMFolder( DefaultConfig.PMFolder() );

    m_gain = DefaultConfig.GlobalGain();

    if( DefaultConfig.ReadbackFlag() )
    {
        //FIXME: maybe config should be read by bvconfig
        m_videoCardManager = new videocards::VideoCardManager();

        m_videoCardManager->RegisterDescriptors( videocards::DefaultVideoCardDescriptors() );
        m_videoCardManager->ReadConfig( DefaultConfig.GetNode( "config" ) );
        m_videoCardManager->Start();

        BVServiceProvider::GetInstance().RegisterVideoCardManager( m_videoCardManager );
    }
}

// *********************************
//
void BVAppLogic::LoadScenes( const PathVec & pathVec )
{
    m_bvProject->GetProjectEditor()->RemoveAllScenes();

    for( auto p : pathVec )
    {
        auto scene = ProjectManager::GetInstance()->LoadScene( "", p );
        m_bvProject->GetProjectEditor()->AddScene( scene );
    }
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    auto projectEditor = m_bvProject->GetProjectEditor();

    //if( !ConfigManager::GetBool( "Debug/LoadSceneFromEnv" ) )
    //{
    //    //if( ConfigManager::GetBool( "Debug/LoadSolution" ) )
    //    //{
    //    //    //m_solution.SetTimeline(m_timelineManager);
    //    //    m_solution.LoadSolution( ConfigManager::GetString("solution") );
    //    //    auto sceneModel = SceneModel::Create( "root" );
    //    //    projectEditor->AddScene( sceneModel );

    //    //    projectEditor->AddChildNode( sceneModel, nullptr, m_solution.GetRoot() );

    //    //    //if(ConfigManager::GetBool("hm"))
    //    //    //root->AddChildToModelOnly(TestScenesFactory::NewModelTestScene( m_pluginsManager, m_timelineManager, m_globalTimeline ));
    //    //}
    //    //else
    //    //{
    //        auto pm = ProjectManager::GetInstance();

    //        auto projectName = ConfigManager::GetString( "default_project_name" );
    //        
    //        if( !projectName.empty() )
    //        {
    //            auto projectScenesNames = pm->ListScenesNames( projectName, "", true );

    //            if( !projectScenesNames.empty() )
    //            {
    //                LoadScenes( projectScenesNames );
    //            }
    //        }
    //    //}
    //}

    auto pmSceneName = DefaultConfig.LoadSceneFromProjectManager();

    if( !pmSceneName.empty() )
    {
        auto pm = ProjectManager::GetInstance();
        auto sceneModel = pm->LoadScene("", pmSceneName);

        if(sceneModel)
        {
            projectEditor->AddScene( sceneModel );
        }
        else 
        {
            // TODO: Add an error log message here.
        }
    }
    else if( DefaultConfig.LoadSceneFromEnv() )
    {
        model::SceneModelPtr sceneModel = nullptr;

        if( GetEnvScene() == "SERIALIZED_TEST" )
        {
            //sceneModel = TestScenesFactory::CreateSerializedTestScene( m_pluginsManager );
            //projectEditor->AddScene( sceneModel );
        }
        else
        {
            auto sceneName = "sceneFromEnv@ " + GetEnvScene();

            sceneModel = model::SceneModel::Create( sceneName );
            projectEditor->AddScene( sceneModel );

            auto defaultTimeline = projectEditor->GetSceneDefaultTimeline( sceneModel );
            auto node = TestScenesFactory::CreateSceneFromEnv( GetEnvScene(), defaultTimeline, m_pluginsManager ) ;
            projectEditor->AddChildNode( sceneModel, nullptr, node );

            defaultTimeline->Play();
        }
    }
}


// *********************************
//
unsigned int BVAppLogic::StartTime       ()
{
    m_timer.Start();
    auto millis = m_timer.ElapsedMillis();
    m_renderMode.SetStartTime( millis );
    m_bvProject->SetStartTime( millis );

    return millis;
}

// *********************************
//
void BVAppLogic::OnUpdate           ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
    HPROFILER_FUNCTION( "BVAppLogic::OnUpdate", PROFILER_THREAD1 );

    m_frameStartTime = m_timer.ElapsedMillis();

    ApplicationContext::Instance().IncrementUpdateCounter();

    GetDefaultEventManager().Update( DefaultConfig.EventLoopUpdateMillis() );

    ApplicationContext::Instance().IncrementUpdateCounter();

    TimeType time = m_renderMode.StartFrame( m_frameStartTime );

    HandleFrame( time, renderer, audioRenderer );
}

// ***********************
//
void BVAppLogic::HandleFrame    ( TimeType time, Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
    assert( m_state != BVAppState::BVS_INVALID );

    if( m_state == BVAppState::BVS_RUNNING )
    {
        FRAME_STATS_FRAME();
        FRAME_STATS_SECTION( DefaultConfig.FrameStatsSection() );

        {
            FRAME_STATS_SECTION( "Update Model" );
            HPROFILER_SECTION( "Update Model", PROFILER_THREAD1 );

            m_bvProject->Update( time );
        }

        // FIXME: shouldn't it be already removed?
        m_remoteHandlers->UpdateHM();

        {
            //m_bvScene->Update( t );
            HPROFILER_SECTION( "Render", PROFILER_THREAD1 );
            
            {
                HPROFILER_SECTION( "Refresh Video Input", PROFILER_THREAD1 );
                FRAME_STATS_SECTION( "Video input" );
                RefreshVideoInputScene();
            }

            {
                static auto last_time = (float) time;

                HPROFILER_SECTION( "Render Frame", PROFILER_THREAD1 );
                FRAME_STATS_SECTION( "Render" );

				audioRenderer->SetGain( m_gain );
                m_renderLogic->HandleFrame( renderer, audioRenderer, m_bvProject->GetScenes() );

                if( time - last_time > 1.1f * m_renderMode.GetFramesDelta() )
                {
                    //printf( "%f, %f, %f, %f, %f \n", last_time, time, m_renderMode.GetFramesDelta(), time - last_time, ( time - last_time ) / m_renderMode.GetFramesDelta() );
                    auto droppedFrames = int(( time - last_time ) / m_renderMode.GetFramesDelta() - 1.0f + 0.01f );
                    LOG_MESSAGE( SeverityLevel::info ) << 
						"DROP: " << 
						last_time * 1000.f << 
						" ms, cur time: " << 
						time * 1000.f << 
						" ms, dropped " << 
						droppedFrames << 
						" frames";
                }

                last_time = time;
            }
        }
    }

    GTimer.StartTimer();
}

// *********************************
//
void BVAppLogic::RefreshVideoInputScene()
{
    //if( DefaultConfig.UseVideoInputFeeding() )
    //{
    //    if( m_videoCardManager->CheckIfNewFrameArrived( 0, "A" ) )
    //    {
    //        BB::AssetManager::VideoInput->RefreshData( m_videoCardManager->GetCaptureBufferForShaderProccessing( 0, "A" ) );
    //    }
    //    else
    //    {
    //        //m_videoCardManager->UnblockCaptureQueue(0,"A");
    //    }
    //}
}

// *********************************
//
void BVAppLogic::OnKey           ( unsigned char c )
{
    m_kbdHandler->HandleKey( c, this );
}

// ***********************
//
unsigned int BVAppLogic::GetTime() const
{
    return m_timer.ElapsedMillis();
}

// ***********************
//
void BVAppLogic::OnMouse         ( MouseAction action, int posX, int posY )
{
    m_kbdHandler->OnMouse( action, posX, posY, this );
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

// ***********************
//
BVAppState      BVAppLogic::GetState        ()
{
    return m_state;
}

// *********************************
//
void BVAppLogic::ShutDown           ()
{
    //TODO: any required deinitialization
    m_remoteController->DeinitializeServer();
    if( m_videoCardManager )
        m_videoCardManager->Stop();
}

// *********************************
//
void    BVAppLogic::PostFrameLogic   ()
{
    if( m_statsCalculator.WasSampledMaxVal( DefaultConfig.FrameStatsSection() ) )
    {


#ifndef HIDE_PROFILE_STATS
                unsigned int frame = m_statsCalculator.CurFrame() - 1;
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

    auto frameMillis = m_timer.ElapsedMillis() - m_frameStartTime;
    if( frameMillis < DefaultConfig.FrameTimeMillis() )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( DefaultConfig.FrameTimeMillis() - frameMillis ) );
    }
}

// *********************************
//
FrameStatsCalculator *  BVAppLogic::GetStatsCalculator  ()
{
    return &m_statsCalculator;
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
    m_bvProject = nullptr;
}

// *********************************
//
void                            BVAppLogic::ReloadScene     ()
{
    ResetScene();
    LoadScene();
}


// *********************************
//FIXME: unsafe - consider returning const variant of this class (IParameters * without const should be accessible anyway)
BVProjectPtr                  BVAppLogic::GetBVProject          () const
{
    return m_bvProject;
}

// *********************************
//
const model::PluginsManager *   BVAppLogic::GetPluginsManager   () const
{
    return m_pluginsManager;
}

// *********************************
//
nrl::NRenderLogic *             BVAppLogic::GetRenderLogic      () const
{
    return m_renderLogic;
}

// *********************************
//
RenderMode &                    BVAppLogic::GetRenderMode        () 
{
    return m_renderMode;
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
    else if( envScene == "GLOBAL_EFFECT_VIDEO_OUTPUT" )
    {
        m_kbdHandler = new TestVideoOutputKeyboardHandler();
    }
    else if( envScene == "SCENE_STRUCTURE_INNER_EVENTS" )
    {
        m_kbdHandler = new TestInnerEvents();
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

    auto editorPort = DefaultConfig.SockerServerPort();
    m_remoteController->InitializeServer( editorPort );
}

// ***********************
//
void                            BVAppLogic::InitializeCommandsDebugLayer()
{
    if( DefaultConfig.UseDebugLayer() )
    {
        m_remoteController->InitializeDebugLayer( DefaultConfig.DebugFilePath() );
    }
}

// *********************************
//
std::string                     BVAppLogic::GetEnvScene()
{
    auto s = DefaultConfig.SceneFromEnvName();
    if( s != "" )
        return s;
    else
        return Env::GetVar( DefaultConfig.DefaultSceneEnvVarName() );
}

// *********************************
//
void							BVAppLogic::SetGain			( Float32 gain )
{
	m_gain = gain;
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
