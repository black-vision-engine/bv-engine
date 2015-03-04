#include "bvApp.h"

#include "System/InitSubsystem.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Processes/ProcessManager.h"
#include "Engine/Models/Resources/ResourceManager.h"
#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Resources/Font/FontLoader.h"
#include "Engine/Models/Resources/Font/FontResourceDescriptor.h"

#include "System/HerarchicalProfiler.h"

#include "StatsFormatters.h"
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
    : WindowedApplication( "BlackVision prealpha test app", 0, 0, DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight(), DefaultConfig.FullScreenMode() )
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
    HPROFILER_NEW_FRAME();
    HPROFILER_FUNCTION( "BlackVisionApp::OnIdle" );

    unsigned long millis = m_timer.ElapsedMillis();

    UpdateSubsystems( millis );

    m_app->OnUpdate( millis, m_timer, m_Renderer, handle );

    PostFrame( millis );
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
    InitializeSelfState     ();

    return WindowedApplication::OnInitialize();
}

// *********************************
//
void BlackVisionApp::OnTerminate        ()
{
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
    if( AllocConsole() )
    {
        FILE * dummy;

        SetConsoleTitleA( "Debug Console" );
    
        freopen_s( &dummy, "CONOUT$", "wb", stdout );
        freopen_s( &dummy, "CONOUT$", "wb", stderr );

        std::cout << sizeof(FILE*);
    }
}

// *********************************
//
void    BlackVisionApp::InitializeAppLogic  ()
{
    HPROFILER_SET_DISPLAY_WAIT_MILLIS( DefaultConfig.ProfilerDispWaitMillis() );
#ifndef HIDE_PROFILE_STATS
    HPROFILER_REGISTER_DISPLAY_CALLBACK( ProfilerDataFormatter::PrintToConsole );
#else
    HPROFILER_REGISTER_DISPLAY_CALLBACK( ProfilerDataFormatter::PrintToDevNull );
#endif

    m_app = new BVAppLogic();

    m_app->Initialize();
    m_app->LoadScene();
    m_app->InitCamera( m_Renderer, m_Width, m_Height );
}

// *********************************
//
void    BlackVisionApp::InitializeSelfState ()
{
    WindowedApplication::SetClearColor( DefaultConfig.ClearColor() );
}

// *********************************
//
void    BlackVisionApp::InitializeResourceLoaders ()
{
    bv::ResourceManager::GetInstance().RegisterLoader( bv::TextureResourceDesc::UID(), new bv::model::TextureLoader() );
	bv::ResourceManager::GetInstance().RegisterLoader( bv::FontResourceDesc::UID(), new bv::model::TextureLoader() );
}


// *********************************
//
void    BlackVisionApp::PostFrame           ( unsigned int millis )
{
#ifndef PRODUCTION_BUILD
    static unsigned int startMillis = millis;

    m_app->PostFrameLogic( m_timer, millis );
    
    if( millis - startMillis > DefaultConfig.StatsRefreshMillisDelta() )
    {
		SetWindowTextW( handle, FrameStatsFormatter::FPSStatsLine( m_app->FrameStats() ).c_str() );
        startMillis = millis;
    }
#endif
}

} //bv
