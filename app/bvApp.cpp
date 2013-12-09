#include "bvApp.h"

#include "System/InitSubsystem.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Processes/ProcessManager.h"

#include "BVAppLogic.h"
#include "BVConfig.h"


bv::IEventManager * GEventManager = nullptr;

bv::FrameRenderedEventPtr  GframeRenderedEvent;
bv::model::SetTextEventPtr GframeSetTextEvent;


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
    GframeSetTextEvent->SetChar( c );
    GEventManager->QueueEvent( GframeSetTextEvent );
}

// *********************************
//
void BlackVisionApp::OnIdle		()
{
    static bool firstPass = true;

    DWORD curTime = timeGetTime();

    if( firstPass )
    {
        firstPass = false;
        m_app->SetStartTime( curTime ); 
    }

    GEventManager->Update( DefaultConfig.EventLoopUpdateMillis() );
    m_processManager->Update( curTime );

    m_app->OnUpdate( curTime, m_Renderer, handle );

    ReadBackFrameBuffer(); //FIXME: move to OnUpdate of BVAppLogic - it is not needed here at all

}

// *********************************
//FIXME: implement proper console
bool BlackVisionApp::OnInitialize       ()
{
    m_processManager = new ProcessManager();

    InitializeConsole       ();
    InitializeAppLogic      ();
    InitializeReadback      ();

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
void    BlackVisionApp::InitializeReadback  ()
{
    //FIXME: temporary hack to enable access to framebuffer readbacks
    GEventManager = &bv::GetDefaultEventManager ();

    GframeRenderedEvent = FrameRenderedEventPtr ( new bv::FrameRenderedEvent( m_Renderer ) );
    GframeSetTextEvent  = model::SetTextEventPtr( new model::SetTextEvent() );
}

// *********************************
//
void BlackVisionApp::ReadBackFrameBuffer ()
{
    if ( !DefaultConfig.ReadbackFlag() )
    {
        return;
    }

    GframeRenderedEvent->SetResolution( m_Width, m_Height );
    GEventManager->TriggerEvent( GframeRenderedEvent );
}

} //bv
