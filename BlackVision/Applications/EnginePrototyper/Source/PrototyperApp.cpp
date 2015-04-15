#include "PrototyperApp.h"

#include <windows.h>

#include <iostream>

#include "System/InitSubsystem.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

#include "PrototypesForwardInc.h"

#include "Tools/HRTimer.h"


namespace bv {

extern HighResolutionTimer GTimer;

// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
void			PrototyperApp::StaticInitializer	()
{
    bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
    bv::ApplicationBase::ApplicationInstance = new PrototyperApp();
}


// *********************************
// FIXME: move it to valid BV windowed version of engine and wrap with a macro
bool			PrototyperApp::RegisterInitializer	()
{
    bv::InitSubsystem::AddInitializer( PrototyperApp::StaticInitializer );

    return true;
}

bool PrototyperApp::m_sWindowedApplicationInitialized = PrototyperApp::RegisterInitializer();


// *********************************
//
PrototyperApp::PrototyperApp	()
    : WindowedApplication( "BlackVision prototyper", 0, 0, 800, 600, false )
    , m_appLogicPrototype( nullptr )
{
}

// *********************************
//
PrototyperApp::~PrototyperApp ()
{
    FreeConsole();

    delete m_appLogicPrototype;
}

// *********************************
//
void PrototyperApp::OnKey( unsigned char c )
{
    m_appLogicPrototype->Key( c );
}

// *********************************
//
void PrototyperApp::OnPreidle  ()
{
}

static auto time = GTimer.CurElapsed();
// *********************************
//
void PrototyperApp::OnIdle		()
{
	auto t = GTimer.CurElapsed();
	std::cout << "Render Time: " << t - time << std::endl;
	time = t;

    static DWORD curMillis = timeGetTime();

    m_appLogicPrototype->Update( float( timeGetTime() - curMillis ) * 0.001f );

	m_appLogicPrototype->Render();

    m_Renderer->DisplayColorBuffer();
	
}

// *********************************
//
void PrototyperApp::OnPreMainLoop  ()
{
}

// *********************************
//
bool PrototyperApp::OnInitialize       ()
{
    bv::Camera * cam = new bv::Camera();

    m_Renderer->SetCamera( cam );

    // Allocate console
    if( AllocConsole() )
    {
        FILE * dummy;

        SetConsoleTitleA( "Debug Console" );
    
        freopen_s( &dummy, "CONOUT$", "wb", stdout );
        freopen_s( &dummy, "CONOUT$", "wb", stderr );
    }

    m_appLogicPrototype = CreateDefaultPrototype( m_Renderer );
    m_appLogicPrototype->Initialize();

    return true;
}

// *********************************
//
void PrototyperApp::OnResize            ( int w, int h )
{
    WindowedApplication::OnResize( w, h );

    m_appLogicPrototype->Resize( (UInt32) w, (UInt32) h );
}

// *********************************
//
void PrototyperApp::OnTerminate        ()
{
}

} // bv
