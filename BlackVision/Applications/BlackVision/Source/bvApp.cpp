#include "bvApp.h"

#include "System/InitSubsystem.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Processes/ProcessManager.h"
#include "Assets/AssetManager.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Texture/AnimationLoader.h"
#include "Assets/Font/FontAssetDescriptor.h"

#include "Tools/HerarchicalProfiler.h"

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
    : WindowedApplication( "BlackVision prealpha test app", 0, 0, DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight(), DefaultConfig.GetWindowMode() )
    , m_processManager( nullptr )
    , m_app( nullptr )
{
}

// *********************************
//
BlackVisionApp::~BlackVisionApp ()
{
	m_videoCardManager.Black();

    m_processManager->AbortAll( true );
    delete m_processManager;

    m_app->ShutDown();
    delete m_app;

    FreeConsole();
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

    m_app->OnUpdate( millis, m_Renderer );

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
		//pablito
    InitializeLogger        ();
    InitializeLicenses      ();
    InitializeSocketServer  ();
    InitializeConfig        ();


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
    }
}

//pablito
// *********************************
//`
void BlackVisionApp::InitializeLogger        ()
{
    //Log::Connect();
    Log::EnableConsoleOutput();
    Log::A(L"Connection Initialized");
}

// *********************************
//
bool    BlackVisionApp::InitializeLicenses   ()
{
    LicenseManager::LoadLicenses();
    bool license = LicenseManager::VerifyLicense();

    Log::A(L"license");
	if(license)
	{
        Log::A(L"tools",L"license",L"License is valid. Proceeding...");
	}else{
        Log::A(L"tools",L"license",L"License is not valid. Please contact your administrator or sales representative");

	}
    return license;
}

// *********************************
//
void    BlackVisionApp::InitializeSocketServer  ()
{
    SocketWrapper Server;
	Server.InitServer();
}

// *********************************
//
void    BlackVisionApp::InitializeConfig  ()
{
    //ConfigManager::LoadConfig();
    ConfigManager::LoadXMLConfig();
    BB::AssetManager::SetMediaFolderPath(ConfigManager::GetString("MediaFolder"));

	BB::AssetManager::LoadSurfaces();

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

    m_app = new BVAppLogic( m_Renderer );

    // FIXME: InitCamera depends implicitly ond LoadScene - which suxx (as camera is created by LoadScene and passed to bvScene)
	m_app->SetVideoCardManager(&m_videoCardManager);
    m_app->Initialize();
    m_app->LoadScene();
    m_app->InitCamera( m_Width, m_Height );
}

// *********************************
//
void    BlackVisionApp::InitializeSelfState ()
{
    WindowedApplication::SetClearColor( DefaultConfig.ClearColor() );
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
