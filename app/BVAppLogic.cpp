#include "BVAppLogic.h"

#include "Engine/Events/Interfaces/IEventManager.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelScene.h"

#include "ModelInteractionEvents.h"

#include "System/HerarchicalProfiler.h"

#include "StatsFormatters.h"
#include "MockScenes.h"

#include "System/SimpleTimer.h"
#include "System/HRTimer.h"
#include "BVConfig.h"

#include "DefaultPlugins.h"

//FIXME: remove
#include "Engine/Models/Plugins/PluginUtils.h"
#include "Engine/Models/Timeline/TimeSegmentEvalImpl.h"
#include "testai/TestAIManager.h"

#include "Engine/Models/Resources/TextureHelpers.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"

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
        //DETERMINSTIC TIME INTERVALS
        //static TimeType tt = TimeType( 0.0 );
        //tt += TimeType( 0.001 );

        //TEST AI
        //static auto ai = TestAIManager::Instance().GetAIPreset( 1 );
        //static auto ai = TestAIManager::Instance().GetAIPreset( 5, logic );
        //static auto ai = TestAIManager::Instance().GetAIPreset( 4, logic );
        //static auto ai = TestAIManager::Instance().GetAIPreset( 2, logic->GetModelScene()->GetSceneRoot() );
        //static auto ai = TestAIManager::Instance().GetAIPreset( 3, logic->GetModelScene()->GetSceneRoot() );
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
    , m_mockSceneEng( nullptr )
    , m_pluginsManager( nullptr )
    , m_offscreenRenderLogic( nullptr )
    , m_state( BVAppState::BVS_INVALID )
    , m_statsCalculator( DefaultConfig.StatsMAWindowSize() )
    , m_globalTimeline( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) )
{
    GTransformSetEvent = TransformSetEventPtr( new TransformSetEvent() );
    GKeyPressedEvent = KeyPressedEventPtr( new KeyPressedEvent() );
    GTimer.StartTimer();

    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() ); 
}

// *********************************
//
BVAppLogic::~BVAppLogic             ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetColorParamEvent::Type() );

    delete m_timelineManager;
    delete m_mockSceneEng;

    delete m_offscreenRenderLogic;
}

// *********************************
//
void BVAppLogic::Initialize         ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetTransformParamsEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &BVAppLogic::OnUpdateParam ), SetColorParamEvent::Type() );

    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
    m_pluginsManager = &model::PluginsManager::DefaultInstance();
}

// *********************************
//
void BVAppLogic::LoadScene          ( void )
{
    //model::BasicNodePtr root = TestScenesFactory::NewModelTestScene( m_pluginsManager, m_timelineManager, m_globalTimeline );
    //model::BasicNodePtr root = TestScenesFactory::OlafTestScene(m_pluginsManager, m_timelineManager, m_globalTimeline);
    model::BasicNodePtr root = TestScenesFactory::CreedTestScene(m_pluginsManager, m_timelineManager, m_globalTimeline);
	assert( root );

    m_mockSceneEng  = root->BuildScene();
    assert( m_mockSceneEng );

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

    m_offscreenRenderLogic->SetRendererCamera( cam );
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
void BVAppLogic::OnUpdate           ( unsigned int millis, const SimpleTimer & timer, Renderer * renderer, HWND handle )
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
                HPROFILER_SECTION( "m_mockSceneEng->Update" );

                auto viewMat = m_modelScene->GetCamera()->GetViewMatrix();

                //FIXME: use transform vector consistenlty
                std::vector< bv::Transform > vec;
                vec.push_back( Transform( viewMat, glm::inverse( viewMat ) ) );
                m_mockSceneEng->Update( vec );
            }
        }
        {
            FRAME_STATS_SECTION( "Render" );
            HPROFILER_SECTION( "Render" );

            renderer->ClearBuffers();
            RenderScene( renderer );
            renderer->DisplayColorBuffer();

            FrameRendered( renderer );
        }
    }

    GTimer.StartTimer();
}

// *********************************
//
void BVAppLogic::OnKey           ( unsigned char c )
{
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
void BVAppLogic::FrameRendered      ( Renderer * renderer )
{
    static int w = 0;
    static int h = 0;

    if( !DefaultConfig.ReadbackFlag() )
    {
        //Not needed as it does not make sense without readback delay
        //m_offscreenRenderLogic->SwapDisplayRenderTargets();
        return;
    }

    if( w != renderer->GetWidth() || h != renderer->GetHeight() )
    {
        w = renderer->GetWidth();
        h = renderer->GetHeight();

        printf( "Framebuffer resolution changed to %dx%d\n", w, h );
    }

    static double totalElapsed = 0.0;
    static int nFrames = 1;
    static int nPasses = 0;
    static int nReadbackFrame = 0;

    double readbackStart = GTimer.CurElapsed();
    auto frame = m_offscreenRenderLogic->ReadDisplayTarget( renderer, nReadbackFrame );
    nReadbackFrame = ( nReadbackFrame + 1 ) % m_offscreenRenderLogic->NumReadBuffersPerRT();
    double readbackTime = GTimer.CurElapsed() - readbackStart;

    m_offscreenRenderLogic->SwapDisplayRenderTargets();

    totalElapsed += readbackTime;

    //printf( "Time cur %.5f ms of total %.5f ms\n", 1000.f * readbackTime, 1000.f * totalElapsed );

    if( nFrames % 50 == 0 )
    {
        double avg = totalElapsed / (double) nFrames;

        nPasses++;
        totalElapsed = 0.0;

        if ( nPasses % 3 == 0 )
        {
            nPasses = 0;

            //printf( "Avg readback time from last %d frames took %.4f ms\n", nFrames, avg * 1000 );
        }

        nFrames = 0;
    }

    nFrames++;
    
    //TODO: code used to push data to playback cards
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
    m_modelScene = nullptr;
    delete m_mockSceneEng;
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
void BVAppLogic::RenderScene     ( Renderer * renderer )
{
    renderer->PreDraw();

    m_offscreenRenderLogic->EnableDisplayRenderTarget( renderer );
    renderer->ClearBuffers();

    RenderNode( renderer, m_mockSceneEng );
    m_offscreenRenderLogic->DisableDisplayRenderTarget( renderer );

    m_offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    renderer->PostDraw();
}

// *********************************
//
void BVAppLogic::RenderNode      ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        bool isOverriden = node->IsOverriden();
       
        //Render to auxiliary buffer
        if( isOverriden )
        {
            assert( m_offscreenRenderLogic->AuxRenderTargetEnabled() == false );
            m_offscreenRenderLogic->EnableAuxRenderTarget( renderer );
            renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
            renderer->ClearBuffers();
            renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
        }

        DrawNode( renderer, node );

        //Blend auxiliary buffer with current 
        if( isOverriden )
        {
            m_offscreenRenderLogic->EnableDisplayRenderTarget( renderer );
            m_offscreenRenderLogic->SetAuxAlphaModelValue( node->GetOverrideAlpha() );
            m_offscreenRenderLogic->DrawAuxRenderTarget( renderer );
        }
    }
}

// *********************************
//
void            BVAppLogic::DrawNode        ( Renderer * renderer, SceneNode * node )
{
    HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor" );
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetAnchor() ) );

    for( int i = 0; i < node->NumTransformables(); ++i )
    {
        HPROFILER_SECTION( "RenderNode::renderer->Draw sibling" );
        renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable( i ) ) );
    }

    for ( int i = 0; i < node->NumChildrenNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode" );
        RenderNode( renderer, node->GetChild( i ) ); 
    }
}

// *********************************
//
void            BVAppLogic::OnUpdateParam   ( IEventPtr evt )
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
