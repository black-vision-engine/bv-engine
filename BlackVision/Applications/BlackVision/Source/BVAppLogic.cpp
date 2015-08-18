#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/BVSceneEditor.h"

#include "Tools/SimpleTimer.h"
#include "Tools/HerarchicalProfiler.h"

#include "Rendering/RenderLogic.h"
#include "ModelInteractionEvents.h"

#include "Widgets/Crawler/CrawlerEvents.h"

#include "BVConfig.h"

#include "MockScenes.h"
#include "DefaultPlugins.h"
#include "LibEffect.h"

//FIXME: remove
#include "testai/TestAIManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "BVGL.h"
//FIXME: end of remove

#include"StatsFormatters.h"

#define _USE_MATH_DEFINES
#include <math.h>

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
{
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
    GKeyPressedEvent = KeyPressedEventPtr( new KeyPressedEvent() );
    GTimer.StartTimer();

    m_renderer = renderer;
    m_renderLogic = new RenderLogic();
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetColorParamEvent::Type() );

    delete m_timelineManager;

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

	bv::effect::InitializeLibEffect( m_renderer );
}


// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    m_timelineManager->RegisterRootTimeline( m_globalTimeline );
    auto root = TestScenesFactory::CreateSceneFromEnv( m_pluginsManager, m_timelineManager, m_globalTimeline );
	assert( root );

    m_bvScene    = BVScene::Create( root, new Camera( DefaultConfig.IsCameraPerspactive() ), "BasicScene", m_globalTimeline, m_renderer );
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

            m_globalTimeline->SetGlobalTime( t );
            m_bvScene->Update( t );
        }
        {
            FRAME_STATS_SECTION( "Render" );
            HPROFILER_SECTION( "Render" );

            m_renderLogic->RenderFrame  ( renderer, m_bvScene->GetEngineSceneRoot() );
            m_renderLogic->FrameRendered( renderer );
        }
    }

    GTimer.StartTimer();
}

// *********************************
//
model::IModelNodePtr BVAppLogic::CreateTestModelNodeInSomeSpecificScope( const std::string & name )
{
    model::BasicNodePtr node = TestScenesFactory::CreateTestRandomNode( name, m_pluginsManager, m_timelineManager, m_globalTimeline );

    return node;
}

// *********************************
//
void BVAppLogic::OnKey           ( unsigned char c )
{
    if( c == '-' )
    {
        BVGL::PrintCompleteSummary( "BEFORE REMOVING ROOT NODE" );
        m_bvScene->GetSceneEditor()->DeleteRootNode();
    }
    else if( c == 8 )
    {
        BVGL::PrintCompleteSummary( "BEFORE REMOVING ROOT NODE" );

        auto root = m_bvScene->GetModelSceneRoot();
        m_bvScene->GetSceneEditor()->DeleteChildNode( root, "child0" );
        //root->DeleteNode( "child0", m_renderer );
        
        //auto child = root->GetChild( "child0" );
        //child->DeleteNode( "child01", m_renderer );
        BVGL::PrintCompleteSummary( "AFTER REMOVING ROOT NODE" );
    }
    else if( c == '+' )
    {
        auto root = m_bvScene->GetModelSceneRoot();
        
        if( root )
        {
            auto child = root->GetChild( "child0" );

            if( child )
            {
                auto n = child->GetNumChildren();
                auto nodeName = "child0" + std::to_string(n);

                auto newNode = CreateTestModelNodeInSomeSpecificScope( nodeName );

                m_bvScene->GetSceneEditor()->AddChildNode( child, newNode );            
            }
            else
            {
                auto newNode = CreateTestModelNodeInSomeSpecificScope( "child0" );
                
                m_bvScene->GetSceneEditor()->AddChildNode( root, newNode );            
            }
        }
        else
        {
            auto newNode = CreateTestModelNodeInSomeSpecificScope( "root node" );
        
            m_bvScene->GetSceneEditor()->SetRootNode( newNode );
        }

        BVGL::PrintCompleteSummary( "AFTER ADD NODE" );
    }
    else if( c == '1' )
    {
        m_bvScene->GetSceneEditor()->DetachRootNode();
        BVGL::PrintCompleteSummary( "AFTER DETACH ROOT NODE" );
    }
    else if( c == '2' )
    {
        m_bvScene->GetSceneEditor()->AttachRootNode();
        BVGL::PrintCompleteSummary( "AFTER ATTACH ROOT NODE" );
    }

    else if( c == '3' )
    {
        auto root = m_bvScene->GetModelSceneRoot();
        
        if( root )
        {
            m_bvScene->GetSceneEditor()->AttachChildNode( root );

            BVGL::PrintCompleteSummary( "AFTER ATTACH NODE TO ROOT" );
        }
    }
    else if( c == 's' )
    {
        auto sob = new SerializeObject();
        m_bvScene->Serialize( *sob );
        sob->Save( "text.xml" );
        delete sob;
    }

/*
    // FIXME: the code below is must be used with an animation plugin
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
*/
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
model::TimelineManager *    BVAppLogic::GetTimelineManager  ()
{
    return m_timelineManager;
}

// *********************************
//FIXME: unsafe - consider returning const variant of this class (IParameters * without const should be accessible anyway)
BVScenePtr                  BVAppLogic::GetBVScene          ()
{
    return m_bvScene;
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

//namespace 
//{
//	const static std::wstring examples[] = 
//	{
//		L"Jasiu kup kiełbasę !!",
//		L"wielojęzyczny projekt internetortej treści. Funkcjonuje wykorzystując",
//		L"Wikipedia powstała 15 stycznia ertów i nieistniejącej już Nupedii. ",
//		L"iostrzane. Wikipedia jest jedną], a wiele stron uruchomiło jej mirrory lub forki.",
//		L"Współzałożyciel Wikipedii Jimmyia wielojęzycznej",
//		L"wolnej encyklopedii o najwyższywłasnym języku”[8].",
//		L"Kontrowersje budzi wiarygodnośćeści artykułów ",
//		L"i brak weryfikacji kompetencji .",
//		L"Z drugiej",
//		L"strony możliwość swobodnej dyst źródłem informacji",
//		L"Jasiu kup kiełbasę !!",
//	};
//
//	auto exampleSize = sizeof( examples ) / sizeof( std::wstring );
//}
