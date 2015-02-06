#include "PrototyperApp.h"

#include <windows.h>

#include <iostream>

#include "System/InitSubsystem.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Prototypes/SimpleVAOPrototype0.h"


namespace bv {

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
    delete m_appLogicPrototype;
}

// *********************************
//
void PrototyperApp::OnKey( unsigned char c )
{
    { c; } // FIXME: suppress unused warning
}

// *********************************
//
void PrototyperApp::OnPreidle  ()
{
}

// *********************************
//
void PrototyperApp::OnIdle		()
{
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

        std::cout << sizeof(FILE*);
    }

    m_appLogicPrototype = new SimpleVAOPrototype0();
    m_appLogicPrototype->Initialize();

    return true;
}

// *********************************
//
void PrototyperApp::OnTerminate        ()
{
}

} // bv
