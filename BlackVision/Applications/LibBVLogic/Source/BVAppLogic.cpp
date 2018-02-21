#include "stdafxBVApp.h"

#include "BVAppLogic.h"

#include "Services/BVServiceProvider.h"

#include "Application/ApplicationContext.h"

// FIXME: nrl - render logic replacement
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"
#include "EndUserAPI/EventHandlers/RemoteEventsHandlers.h"

#include "VideoCardManager.h"

#include "Statistics/StatsFormatters.h"

#include "UseLoggerBVAppModule.h"


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
BVAppLogic::~BVAppLogic             ()
{
    if( m_renderLogic )
        delete m_renderLogic;
    
    delete m_remoteHandlers;
    delete m_kbdHandler;

    delete m_remoteController;
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


// ========================================================================= //
// Update and frame handling
// ========================================================================= //


// *********************************
// Note: The same loop is used in test framework, so remember to modify FrameworkTest::RunImplNotConst function either.
void            BVAppLogic::OnUpdate           ( Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
    HPROFILER_FUNCTION( "BVAppLogic::OnUpdate", PROFILER_THREAD1 );

    FRAME_STATS_FRAME();
    FRAME_STATS_SECTION( DefaultConfig.FrameStatsSection() );

    TimeType time = ComputeFrameTime();

    EventsPhase();
    ModelUpdatePhase( time );
    RenderPhase( time, renderer, audioRenderer );

    GTimer.StartTimer();
}

// ***********************
//
TimeType        BVAppLogic::ComputeFrameTime    ()
{
    m_frameStartTime = m_timer.ElapsedMillis();
    return m_renderMode.StartFrame( m_frameStartTime );
}

// ***********************
//
void            BVAppLogic::EventsPhase         ()
{
    ApplicationContext::Instance().IncrementUpdateCounter();

    GetDefaultEventManager().Update( DefaultConfig.EventLoopUpdateMillis() );

    ApplicationContext::Instance().IncrementUpdateCounter();
}

// ***********************
//
void            BVAppLogic::ModelUpdatePhase    ( TimeType time )
{
    assert( m_state != BVAppState::BVS_INVALID );

    if( m_state == BVAppState::BVS_RUNNING )
    {
        FRAME_STATS_SECTION( "Update Model" );
        HPROFILER_SECTION( "Update Model", PROFILER_THREAD1 );

        m_bvProject->Update( time );
    }
}

// ***********************
//
void            BVAppLogic::RenderPhase         ( TimeType time, Renderer * renderer, audio::AudioRenderer * audioRenderer )
{
    assert( m_state != BVAppState::BVS_INVALID );

    if( m_state == BVAppState::BVS_RUNNING )
    {
        //m_bvScene->Update( t );
        HPROFILER_SECTION( "Render", PROFILER_THREAD1 );

        {
            HPROFILER_SECTION( "Refresh Video Input", PROFILER_THREAD1 );
            FRAME_STATS_SECTION( "Video input" );
            RefreshVideoInputScene();
        }

        {
            HPROFILER_SECTION( "Render Frame", PROFILER_THREAD1 );
            FRAME_STATS_SECTION( "Render" );

            audioRenderer->SetGain( m_gain );
            m_renderLogic->HandleFrame( renderer, audioRenderer, m_bvProject->GetScenes() );

            { time; }
            //CheckDropFrame( time );
        }
    }
}

// ***********************
//
void            BVAppLogic::CheckDropFrame      ( TimeType time )
{
    static auto lastTime = ( float )time;

    if( time - lastTime > 1.1f * m_renderMode.GetFramesDelta() )
    {
        //printf( "%f, %f, %f, %f, %f \n", lastTime, time, m_renderMode.GetFramesDelta(), time - lastTime, ( time - lastTime ) / m_renderMode.GetFramesDelta() );
        auto droppedFrames = int( ( time - lastTime ) / m_renderMode.GetFramesDelta() - 1.0f + 0.01f );
        LOG_MESSAGE( SeverityLevel::info ) <<
            "DROP: " <<
            lastTime * 1000.f <<
            " ms, cur time: " <<
            time * 1000.f <<
            " ms, dropped " <<
            droppedFrames <<
            " frames";
    }

    lastTime = time;
}

// *********************************
//
void							BVAppLogic::SetGain			( Float32 gain )
{
    m_gain = gain;
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

// ========================================================================= //
// Input, state handling and post frame logic
// ========================================================================= //

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
void            BVAppLogic::Deinitialize    ()
{
    m_remoteController->DeinitializeServer();

    if( m_videoCardManager )
    {
        m_videoCardManager->Stop();

        BVServiceProvider::GetInstance().UnregisterVideoCardManager();
        delete m_videoCardManager;
        m_videoCardManager = nullptr;
    }

    delete m_renderLogic;
    m_renderLogic = nullptr;
}

// *********************************
//
void            BVAppLogic::ShutDown        ()
{
    //TODO: any required deinitialization
    if( m_videoCardManager )
        m_videoCardManager->Stop();
}

// *********************************
//
void            BVAppLogic::PostFrameLogic   ()
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
RenderLogic *                   BVAppLogic::GetRenderLogic      () const
{
    return m_renderLogic;
}

// *********************************
//
RenderMode &                    BVAppLogic::GetRenderMode        () 
{
    return m_renderMode;
}



} //bv
