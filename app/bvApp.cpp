#include "bvApp.h"

#include "System/InitSubsystem.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Processes/ProcessManager.h"

#include "System/HerarchicalProfiler.h"

#include "BVAppLogic.h"
#include "BVConfig.h"


// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
void			bv::BlackVisionApp::StaticInitializer	()
{
    bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
    bv::ApplicationBase::ApplicationInstance = new bv::BlackVisionApp();
}


// *********************************
// FIXME: move it to valid BV windowed version of engine and wrap with a macro
bool			bv::BlackVisionApp::RegisterInitializer	()
{
    bv::InitSubsystem::AddInitializer( bv::BlackVisionApp::StaticInitializer );

    return true;
}

bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionApp::RegisterInitializer();


namespace bv {

// *********************************
//
BlackVisionApp::BlackVisionApp	()
    : WindowedApplication( "BlackVision prealpha test app", 0, 0, DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.FullScreenMode() )
    , m_processManager( nullptr )
    , m_app( nullptr )
{
}

// *********************************
//
BlackVisionApp::~BlackVisionApp ()
{
    m_processManager->AbortAll( true );
    delete m_processManager;

    m_app->ShutDown();
    delete m_app;
}

// *********************************
//
void BlackVisionApp::OnKey( unsigned char c )
{
    m_app->OnKey( c );
}

// *********************************
//
void BlackVisionApp::OnPreidle  ()
{
    m_timer.Start();
    m_app->SetStartTime( m_timer.ElapsedMillis() );
    m_app->ChangeState( BVAppState::BVS_RUNNING );    
}

// *********************************
//
void BlackVisionApp::OnIdle		()
{
    {
        HPROFILER_NEW_FRAME();

        HPROFILER_FUNCTION( "BlackVisionApp::OnIdle" );

        unsigned long millis = m_timer.ElapsedMillis();

        {
            HPROFILER_SECTION( "UpdateSubsystems" );

            UpdateSubsystems( millis );
        }

        m_app->OnUpdate( millis, m_Renderer, handle );

    }

    PostFrame();
}

// *********************************
//
void BlackVisionApp::OnPreMainLoop  ()
{
    m_timer.Start();
    m_app->SetStartTime( m_timer.ElapsedMillis() );
}

// *********************************
//FIXME: implement proper console and console handler
bool BlackVisionApp::OnInitialize       ()
{
    m_processManager = new ProcessManager();

    InitializeConsole       ();
    InitializeAppLogic      ();

    return WindowedApplication::OnInitialize();
}

// *********************************
//
void BlackVisionApp::OnTerminate        ()
{
    model::TimelineManager::Cleanup();
    WindowedApplication::OnTerminate();
}

// *********************************
//
void    BlackVisionApp::UpdateSubsystems    ( unsigned long millis )
{
    GetDefaultEventManager().Update( DefaultConfig.EventLoopUpdateMillis() );

    m_processManager->Update( millis );
}

// *********************************
//
void    BlackVisionApp::InitializeConsole   ()
{
    if( AllocConsole())
    {
        FILE * dummy;

        SetConsoleTitleA( "Debug Console" );
    
        freopen_s( &dummy, "CONOUT$", "wb", stdout );
        freopen_s( &dummy, "CONOUT$", "wb", stderr );
    }
}

// *********************************
//
void    BlackVisionApp::InitializeAppLogic  ()
{
    m_app = new BVAppLogic();

    m_app->Initialize();
    m_app->LoadScene();
    m_app->InitCamera( m_Renderer, m_Width, m_Height );
}

// *********************************
//
void    BlackVisionApp::PostFrame           ()
{
#ifndef PRODUCTION_BUILD
    m_app->PostFrameLogic();
    
    static float totalTime = 0.0;

    //totalTime += stats.frameMillis;

    if( totalTime > 200.f )
    {
        std::ostringstream  s;
        std::cout.precision( 4 );
        //s << "FPS: " << stats.fps << " frame time: " << stats.frameMillis << " ms " << std::endl;
        
		std::string ss = s.str();
		std::wstring stemp = std::wstring( ss.begin(), ss.end() );
		LPCWSTR sw = stemp.c_str();
		SetWindowTextW( handle,sw );

        totalTime = 0.f;
    }
#endif
}

} //bv
