#include "BasicWindowApp.h"

#include <win_sock.h>

#include <iostream>

#include "System/InitSubsystem.h"

#include "System/Time.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Tools/HRTimer.h"

#include "Application/ApplicationContext.h"

namespace bv {

extern HighResolutionTimer GTimer;

// *********************************
//
ApplicationBase *	BasicWindowApp::AppInstance = nullptr;

// *********************************
//
bool			BasicWindowApp::RegisterInitializer	( CreateLogicFunc logicFunc, const char * title, int width, int height, bool fullScreen, int xOffset , int yOffset )
{
	if( AppInstance ) delete AppInstance;
	AppInstance = new BasicWindowApp( logicFunc, title, xOffset, yOffset, width, height, fullScreen );

    bv::InitSubsystem::AddInitializer( []( int, char * [] ) {
		bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
		bv::ApplicationBase::ApplicationInstance = AppInstance;
	});

    return true;
}

// ****************************
//
bool			BasicWindowApp::RegisterConsoleInitializer		()
{
    bv::InitSubsystem::AddInitializer( []( int, char *[] ){
		if( AllocConsole() )
		{
			FILE * dummy;

			SetConsoleTitleA( "Debug Console" );
    
			freopen_s( &dummy, "CONOUT$", "wb", stdout );
			freopen_s( &dummy, "CONOUT$", "wb", stderr );
		}
	});
	return true;
}

// *********************************
//
BasicWindowApp::BasicWindowApp	( CreateLogicFunc logicFunc, const char * title, int xOffset, int yOffset, int width, int height, bool fullScreen )
    : WindowedApplication( title, xOffset, yOffset, width, height, fullScreen ? FULLSCREEN : WINDOWED, *(new bv::RendererInput) )  // FIXME: pablito
	, CreateLogic( logicFunc )
{
}

// *********************************
//
BasicWindowApp::~BasicWindowApp ()
{
    FreeConsole();
}

// *********************************
//
void BasicWindowApp::OnKey( unsigned char c )
{
    m_appLogic->Key( c );
}

// *********************************
//
void BasicWindowApp::OnPreidle  ()
{
}

// *********************************
//
bool BasicWindowApp::OnIdle		()
{
    static auto curMillis = Time::Now();
	auto timeDiff = Time::Now() - curMillis;

	ApplicationContext::Instance().SetUpdateCounter( timeDiff );

    m_appLogic->Update( float( timeDiff ) * 0.001f );

	m_appLogic->Render();

    m_Renderer->DisplayColorBuffer();
	
    return true;
}

// *********************************
//
void BasicWindowApp::OnPreMainLoop  ()
{
}

// *********************************
//
bool BasicWindowApp::OnInitialize       ()
{
    bv::Camera * cam = new bv::Camera();

    m_Renderer->SetCamera( cam );

	m_appLogic = CreateLogic( m_Renderer, m_audioRenderer );
	m_appLogic->Initialize();

    return true;
}

// *********************************
//
void BasicWindowApp::OnResize            ( int w, int h )
{
    WindowedApplication::OnResize( w, h );

    m_appLogic->Resize( (UInt32) w, (UInt32) h );
}

// *********************************
//
void BasicWindowApp::OnTerminate        ()
{
}

} // bv
