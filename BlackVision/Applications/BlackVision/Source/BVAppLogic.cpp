#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "Tools/SimpleTimer.h"
#include "Tools/HerarchicalProfiler.h"

#include "Rendering/RenderLogic.h"
#include "ModelInteractionEvents.h"

#include "Widgets/Crawler/CrawlerEvents.h"

#include "BVConfig.h"

#include "MockScenes.h"
#include "DefaultPlugins.h"

//FIXME: remove
#include "testai/TestAIManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
//FIXME: end of remove

#define _USE_MATH_DEFINES
#include <math.h>

namespace 
{
	const static std::wstring examples[] = 
	{
		L"Jasiu kup kiełbasę !!",
		L"wielojęzyczny projekt internetortej treści. Funkcjonuje wykorzystując",
		L"Wikipedia powstała 15 stycznia ertów i nieistniejącej już Nupedii. ",
		L"iostrzane. Wikipedia jest jedną], a wiele stron uruchomiło jej mirrory lub forki.",
		L"Współzałożyciel Wikipedii Jimmyia wielojęzycznej",
		L"wolnej encyklopedii o najwyższywłasnym języku”[8].",
		L"Kontrowersje budzi wiarygodnośćeści artykułów ",
		L"i brak weryfikacji kompetencji .",
		L"Z drugiej",
		L"strony możliwość swobodnej dyst źródłem informacji",
		L"Jasiu kup kiełbasę !!",
	};

	auto exampleSize = sizeof( examples ) / sizeof( std::wstring );
}

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

// *********************************
//
BVAppLogic::BVAppLogic              ()
    : m_startTime( 0 )
    , m_timelineManager( new model::TimelineManager() )
    , m_modelScene( nullptr )
    , m_engineScene( nullptr )
    , m_pluginsManager( nullptr )
    , m_renderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
    , m_globalTimeline( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) )
{
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
    GKeyPressedEvent = KeyPressedEventPtr( new KeyPressedEvent() );
    GTimer.StartTimer();

    m_renderLogic = new RenderLogic();
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetColorParamEvent::Type() );

    delete m_timelineManager;
    delete m_engineScene;

    delete m_renderLogic;
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

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
    m_pluginsManager = &model::PluginsManager::DefaultInstance();
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    //model::BasicNodePtr root = TestScenesFactory::NewModelTestScene( m_pluginsManager, m_timelineManager, m_globalTimeline );
    //model::BasicNodePtr root = TestScenesFactory::OlafTestScene(m_pluginsManager, m_timelineManager, m_globalTimeline);
    //model::BasicNodePtr root = TestScenesFactory::CreedTestScene(m_pluginsManager, m_timelineManager, m_globalTimeline);
    model::BasicNodePtr root = TestScenesFactory::CreateTestScene( m_pluginsManager, m_timelineManager, m_globalTimeline, TestScenesFactory::TestSceneSelector::TSS_TWO_TEXTURED_RECTANGLES );

	assert( root );

    m_engineScene  = root->BuildScene();
    assert( m_engineScene );

    m_modelScene    = model::ModelScene::Create( root, new Camera( DefaultConfig.IsCameraPerspactive() ), "BasicScene", m_globalTimeline );
    assert( m_modelScene );
}

// *********************************
//
void BVAppLogic::InitCamera         ( Renderer * renderer, unsigned int w, unsigned int h )
{
    Camera * cam = m_modelScene->GetCamera();

    cam->SetFrame( DefaultConfig.CameraPosition(), DefaultConfig.CameraDirection(), DefaultConfig.CameraUp() );
    
    if( cam->IsPerspective() )
    {
        cam->SetPerspective( DefaultConfig.FOV(), w, h, DefaultConfig.NearClippingPlane(), DefaultConfig.FarClippingPlane() );
    }
    else
    {
        cam->SetViewportSize( w, h );
    }

    renderer->SetCamera( cam );
    m_renderLogic->SetCamera( cam );

    //FIXME: read from configuration file and change appropriately when resoultion changes
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
    HPROFILER_FUNCTION( "BVAppLogic::OnUpdate" );

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
            HPROFILER_SECTION( "update total" );

            {
                FRAME_STATS_SECTION( "Model-u" );
                HPROFILER_SECTION( "m_modelScene->Update" );

                m_globalTimeline->SetGlobalTime( t );
                m_modelScene->Update( t );
            }
            {
                FRAME_STATS_SECTION( "Manager-u" );
                HPROFILER_SECTION( "UpdatersManager::Get().UpdateStep" );
                UpdatersManager::Get().UpdateStep();
            }
            {
                FRAME_STATS_SECTION( "EngScn-u" );
                HPROFILER_SECTION( "m_engineScene->Update" );

                auto viewMat = m_modelScene->GetCamera()->GetViewMatrix();

                //FIXME: use transform vector consistenlty
                std::vector< bv::Transform > vec;
                vec.push_back( Transform( viewMat, glm::inverse( viewMat ) ) );
                m_engineScene->Update( vec );
            }
        }
        {
            FRAME_STATS_SECTION( "Render" );
            HPROFILER_SECTION( "Render" );

            m_renderLogic->RenderFrame  ( renderer, m_engineScene );
            m_renderLogic->FrameRendered( renderer );
        }
    }

    GTimer.StartTimer();
}

// *********************************
//
void BVAppLogic::OnKey           ( unsigned char c )
{
    unsigned char d = c - '0';

    if( d <= 10 )
    {
        auto root = m_modelScene->GetSceneRoot();

        SetParameter( root->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 0.f ), float( d ) );
    }

    if( c == 'i' || c == 'I' )
    {
        m_renderLogic->PrintGLStats( c == 'I' );
    }

    //auto root = m_modelScene->GetSceneRoot();
    //auto timerPlugin = root->GetPlugin("timer");
    //if(c == 'q')
    //{
    //    model::StartTimerPlugin( timerPlugin );
    //}

    //if(c == 'w')
    //{
    //    model::StopTimerPlugin( timerPlugin );
    //}
    //   

    //if(c == 'a')
    //{
    //    model::SetTimeTimerPlugin( timerPlugin, 3600.f * 5 + 60.f * 4 + 23.f + 0.12f );
    //}

    //if(c == 's')
    //{
    //    model::SetTimeTimerPlugin( timerPlugin, 43.f + 0.88f );
    //}
        
    //FIXME: keypressed event was used here to set text in all currently loaded Text plugins
    //KeyPressedSendEvent( c );
    //TODO: implement whatever you want here
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

// *********************************
//
void    BVAppLogic::PostFrameLogic   ( const SimpleTimer & timer, unsigned int millis )
{
    if( m_statsCalculator.WasSampledMaxVal( DefaultConfig.FrameStatsSection() ) )
    {
        //unsigned int frame = m_statsCalculator.CurFrame() - 1;
        
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
    m_modelScene = nullptr;
    delete m_engineScene;
}

// *********************************
//
void                            BVAppLogic::ReloadScene     ()
{
    ResetScene();
    LoadScene();
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
	auto typedEvent = std::static_pointer_cast< widgets::NoMoreNodesCrawlerEvent >( evt );
	// Remove code below. Only for testing.
	auto n = typedEvent->GetCrawler()->GetNonActiveNode();
	if( n )
	{
		auto i = rand() % exampleSize;
		auto textNode = n->GetChild( "Text" );
		if( textNode )
		{
			auto pl = textNode->GetPlugin( "text" );

			if( pl )
			{
				model::DefaultTextPlugin::SetText( pl, examples[ i ] );

				typedEvent->GetCrawler()->EnqueueNode( n );
			}
		}
	}
}

// *********************************
//
model::TimelineManager *    BVAppLogic::GetTimelineManager  ()
{
    return m_timelineManager;
}

// *********************************
//FIXME: unsafe - consider returning const variant of this class (IParameters * without const should be accessible anyway)
model::ModelScenePtr        BVAppLogic::GetModelScene       ()
{
    return m_modelScene;
}

// *********************************
//
const model::PluginsManager *   BVAppLogic::GetPluginsManager   () const
{
    return m_pluginsManager;
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
