#pragma warning(disable :4996)
#include "bvApp.h"

#include "System/InitSubsystem.h"
#include "EndUserAPI/RemoteController.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Processes/ProcessManager.h"
#include "Assets/AssetManager.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Texture/AnimationLoader.h"
#include "Assets/Font/FontAssetDescriptor.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

#include "StatsFormatters.h"
#include "BVAppLogic.h"
#include "BVConfig.h"

#include "Application/ApplicationContext.h"
#include "Application/Win32/DisableCrashReport.h"

// Log initializer
#include "bvAppLogInitializer.inl"

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
    bv::InitSubsystem::AddInitializer( bv::BlackVisionApp::LoggerInitializer );

    return true;
}

bool bv::BlackVisionApp::m_sWindowedApplicationInitialized = bv::BlackVisionApp::RegisterInitializer();

namespace bv {

// *********************************
//
BlackVisionApp::BlackVisionApp	()
    : WindowedApplication( "BlackVision prealpha test app", 0, 0, DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight(), DefaultConfig.GetWindowMode(), DefaultConfig.GetRendererInput() )
    , m_processManager( nullptr )
    , m_app( nullptr )
{
    ApplicationContext::Instance().SetResolution( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight() );
}

// *********************************
//
BlackVisionApp::~BlackVisionApp ()
{
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

// ***********************
//
void BlackVisionApp::OnMouse    ( MouseAction action, int posX, int posY )
{
    m_app->OnMouse( action, posX, posY );
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
bool BlackVisionApp::OnIdle		()
{
    HPROFILER_NEW_FRAME( PROFILER_THREAD1 );
    HPROFILER_FUNCTION( "BlackVisionApp::OnIdle", PROFILER_THREAD1 );

    unsigned long millis = m_timer.ElapsedMillis();

    UpdateSubsystems( millis );

    ApplicationContext::Instance().SetTimestamp( millis );

    m_app->OnUpdate( millis, m_Renderer, m_audioRenderer );

    PostFrame( millis );

    if( m_app->GetState() == BVAppState::BVS_CLOSING )
        return false;
    return true;
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
    InitializeLicenses      ();
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

// ***********************
//
void	BlackVisionApp::OnResize        ( int w, int h )
{
    WindowedApplication::OnResize( w, h );
    ApplicationContext::Instance().SetResolution( w, h );
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


// *********************************
//
bool    BlackVisionApp::InitializeLicenses   ()
{
    LicenseManager::LoadLicenses();
    bool license = LicenseManager::VerifyLicense();

    if( license )
    {
        LOG_MESSAGE( SeverityLevel::info ) << "License is valid. Proceeding...";
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::critical ) << "License is not valid. Please contact your administrator or sales representative";
    }
    return license;
}


// *********************************
//
void    BlackVisionApp::InitializeConfig  ()
{
    // Function BVConfig::Instance() have already initialized this config.
    // Code commented to avoid double keys in config.
    //ConfigManager::LoadConfig();
    //ConfigManager::LoadXMLConfig();
    BB::AssetManager::SetMediaFolderPath(ConfigManager::GetString("MediaFolder"));

    BB::AssetManager::LoadSurfaces();

}

// *********************************
//
void    BlackVisionApp::InitializeAppLogic  ()
{
    std::wstring commandLineString = GetCommandLineW();

    if( IsProfilerEnabled( commandLineString ) )
    {
        HPROFILER_REGISTER_DISPLAY_CALLBACK( ProfilerDataFormatter::SendToExternApp );
        HPROFILER_SET_DISPLAY_MODE( ProfilerMode::PM_EVERY_FRAME );
    }
    else
    {
        HPROFILER_REGISTER_DISPLAY_CALLBACK( ProfilerDataFormatter::PrintToDevNull );
        HPROFILER_SET_DISPLAY_MODE( ProfilerMode::PM_WAIT_TIME_AND_FORCE_DISPLAY );
        HPROFILER_SET_DISPLAY_WAIT_MILLIS( DefaultConfig.ProfilerDispWaitMillis() );
    }

    if( IsDisableCrashReportFlagSet( commandLineString ) )
    {
        DisableCrashReport();
    }

    m_app = new BVAppLogic( m_Renderer, m_audioRenderer );

    m_app->SetVideoCardManager( &m_videoCardManager );
    m_app->Initialize();
    m_app->LoadScene();
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
