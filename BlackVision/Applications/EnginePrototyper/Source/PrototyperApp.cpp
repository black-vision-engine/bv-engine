#include "PrototyperApp.h"

#include "System/InitSubsystem.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/Camera.h"


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
    : WindowedApplication( "BlackVision prealpha test app", 0, 0, 800, 600, false )
{
}

// *********************************
//
PrototyperApp::~PrototyperApp ()
{
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

    return true;
}

// *********************************
//
void PrototyperApp::OnTerminate        ()
{
}
