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
#include "testai/TestAIManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "BVGL.h"
//FIXME: end of remove

#include"StatsFormatters.h"

#define _USE_MATH_DEFINES
#include <math.h>

//pablito
#define XML
#include "ConfigManager.h"
#include "RemoteControlInterface.h"


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
    , m_timelineManager( new model::TimelineManager() )
    , m_bvScene( nullptr )
    , m_pluginsManager( nullptr )
    , m_renderer( nullptr )
    , m_renderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
    , m_globalTimeline( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) )
    , m_solution( GetTimelineManager() ) //pablito
{
    model::TimelineManager::SetInstance( GetTimelineManager() );
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
    GKeyPressedEvent = KeyPressedEventPtr( new KeyPressedEvent() );
    GTimer.StartTimer();

    m_renderer = renderer;
    m_renderLogic = new RenderLogic();
	m_RemoteControl = new RemoteControlInterface(this);
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetColorParamEvent::Type() );

    //delete m_timelineManager;

    delete m_renderLogic;

    delete m_kbdHandler;
}

// *********************************
//
void BVAppLogic::Initialize         ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetColorParamEvent::Type() );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnNodeAppearing ), widgets::NodeAppearingCrawlerEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnNodeLeaving ), widgets::NodeLeavingCrawlerEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnNoMoreNodes ), widgets::NoMoreNodesCrawlerEvent::Type() );

    GetTimelineManager()->RegisterRootTimeline( m_globalTimeline );

	model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
    m_pluginsManager = &model::PluginsManager::DefaultInstance();

	bv::effect::InitializeLibEffect( m_renderer );

    InitializeKbdHandler();
}

// *********************************
//
model::BasicNodePtr BVAppLogic::LoadScenes( const PathVec & pathVec )
{
    auto root = model::BasicNode::Create( "root", m_globalTimeline );
    root->AddPlugin( "DEFAULT_TRANSFORM", "transform", m_globalTimeline ); 

    for( auto p : pathVec )
    {
        auto scene = SceneDescriptor::LoadScene( ProjectManager::GetInstance()->ToAbsPath( p ), GetTimelineManager() );

        root->AddChildToModelOnly( std::const_pointer_cast< model::BasicNode >( scene ) );
    }

    m_bvScene    = BVScene::Create( root, new Camera( DefaultConfig.IsCameraPerspactive() ), "BasicScene", m_globalTimeline, m_renderer, GetTimelineManager() );
    InitCamera( 500, 500 );
    assert( m_bvScene );

    return root;
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

     model::BasicNodePtr root;
    
    if( !ConfigManager::GetBool( "Debug/LoadSceneFromEnv" ) )
    {
        if( ConfigManager::GetBool( "Debug/LoadSolution" ) )
        {
            //m_solution.SetTimeline(m_timelineManager);
            m_solution.LoadSolution( ConfigManager::GetString("solution") );
            root = m_solution.GetRoot();
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
                    root = LoadScenes( projectScenesNames );
                }
            }
            else
            {
                root = model::BasicNode::Create ("root", m_globalTimeline);
                root->AddPlugin( "DEFAULT_TRANSFORM", "transform", m_globalTimeline ); 
            }
        }
    }
    else
    {
        root = TestScenesFactory::CreateSceneFromEnv( GetEnvScene(), m_pluginsManager, GetTimelineManager(), m_globalTimeline );
    }

    if( !m_bvScene )  // FIXME: 
    {
        m_bvScene    = BVScene::Create( root, new Camera( DefaultConfig.IsCameraPerspactive() ), "BasicScene", te, m_renderer, GetTimelineManager() );
    }

    assert( m_bvScene );
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

		m_RemoteControl->UpdateHM();

        {
            FRAME_STATS_SECTION( "Render" );
			HPROFILER_SECTION( "Render", PROFILER_THREAD1 );			
            
			RefreshVideoInputScene();

            m_renderLogic->RenderFrame( renderer, m_bvScene->GetEngineSceneRoot() );
            m_renderLogic->FrameRendered( renderer );
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
//pablito
// *********************************
//
void            BVAppLogic::SetKey(  bool active)
{
    m_videoCardManager->SetKey(active);
}

// *********************************
//
void            BVAppLogic::OnUpdateParam   ( IEventPtr evt )
{ 
}

// *********************************
//
void            BVAppLogic::OnNodeAppearing   ( IEventPtr evt )
{ 
}

// *********************************
//
void            BVAppLogic::OnNodeLeaving   ( IEventPtr evt )
{
}

// *********************************
//
void            BVAppLogic::OnNoMoreNodes   ( IEventPtr evt )
{
}

// *********************************
//
model::TimelineManager *    BVAppLogic::GetTimelineManager      ()
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
    if( envScene == "SERIALIZED_TEST" )
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
