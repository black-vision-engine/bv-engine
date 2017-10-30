#include "stdafx.h"

#include "Application/WindowedApplication.h"

#include "System/InitSubsystem.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Audio/AudioRenderer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"

#include <Windows.h>


LRESULT CALLBACK DefaultWindowEventHandler ( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam );



#include "Memory/MemoryLeaks.h"



namespace bv {

namespace {

    HINSTANCE GHInstance;

    bool GWindowDestroyed			= false;
    const int GNumPelBits			= 32;
    const wchar_t sWindowClass[]	= L"Black Vision Window App - OpenGL";

}

// ***********************
//
RECT        GetClientRect   ( HWND handle, int w, int h )
{
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = w;
    rect.bottom = h;

    GetClientRect( handle, &rect );
    return rect;
}

namespace LocalWindowHandler
{

// *********************************
// TODO: implement more handlers if required (and add them to windowed application callbacks interface)
LRESULT CALLBACK DefaultWindowEventHandler ( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{
    WindowedApplication * app = static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance );

    if ( !app || !app->WindowId() )
    {
        return DefWindowProc( handle, msg, wParam, lParam );
    }

    switch ( msg ) 
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint( handle, &ps );
            app->OnDisplay();
            EndPaint( handle, &ps );
            return 0;
        }
        case WM_ERASEBKGND:
        {
            // This tells Windows not to erase the background (and that the
            // application is doing so).
            return 1;
        }
        case WM_MOVE:
        {
            int xPos = (int) ( LOWORD(lParam) );
            int yPos = (int) ( HIWORD(lParam) );
            app->OnMove( xPos, yPos );
            return 0;
        }
        case WM_SIZE:
        {
            int w = (int)(LOWORD(lParam));
            int h = (int)(HIWORD(lParam));

            RECT rect = GetClientRect( handle, w, h );

            app->OnResize( rect.right, rect.bottom );
            return 0;
        }
        case WM_CHAR:
        {
            unsigned char key = (unsigned char)(char)wParam;

            //FIXME: possibly use other key
            if ( key == VK_ESCAPE )
            {
                PostQuitMessage(0);
                return 0;
            }

            app->OnKey( key );

            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            int w = (int)(LOWORD(lParam));
            int h = (int)(HIWORD(lParam));
            app->OnMouse( MouseAction::LEFT_DOWN, w, h );
            return 0;
        }
        case WM_LBUTTONUP:
        {
            int w = (int)(LOWORD(lParam));
            int h = (int)(HIWORD(lParam));
            app->OnMouse( MouseAction::LEFT_UP, w, h );
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            int w = (int)(LOWORD(lParam));
            int h = (int)(HIWORD(lParam));
            app->OnMouse( MouseAction::RIGHT_DOWN, w, h );
            return 0;
        }
        case WM_RBUTTONUP:
        {
            int w = (int)(LOWORD(lParam));
            int h = (int)(HIWORD(lParam));
            app->OnMouse( MouseAction::RIGHT_UP, w, h );
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            int w = (int)(LOWORD(lParam));
            int h = (int)(HIWORD(lParam));
            app->OnMouse( MouseAction::MOVE, w, h );
            return 0;
        }
        case WM_DESTROY:
        {
            GWindowDestroyed = true;
            PostQuitMessage( 0 );
            return 0;
        }
    }

    return DefWindowProc( handle, msg, wParam, lParam );
}

}

namespace {

// *********************************
//
HWND CreateApplicationWindow ( WindowedApplication * app )
{
    WNDCLASS wc;

    GHInstance			= GetModuleHandle( NULL );
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc		= LocalWindowHandler::DefaultWindowEventHandler;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= GHInstance;
    wc.hIcon			= LoadIcon(GHInstance, IDI_APPLICATION);
    wc.hCursor			= LoadCursor(0, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName	= sWindowClass;
    wc.lpszMenuName		= 0;
  
    if ( !RegisterClass( &wc ) )
    {
        MessageBox( NULL, L"Failed to register the window class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
        return NULL;										
    }


    if ( app->FullScreen() )
    {
        DEVMODE dmScreenSettings;								

        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );	
        
        dmScreenSettings.dmSize			= sizeof( dmScreenSettings );		
        dmScreenSettings.dmPelsWidth	= app->Width();				
        dmScreenSettings.dmPelsHeight	= app->Height();				
        dmScreenSettings.dmBitsPerPel	= GNumPelBits;					
        dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        
        if ( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
        {
            
            if ( MessageBox( NULL, L"Fulscreen mode not available. Switch to windowed version?", sWindowClass, MB_YESNO | MB_ICONEXCLAMATION ) == IDYES )
            {
                app->UnsetFulscreen();
            }
            else
            {            
                return NULL;							
            }
        }
    }

    DWORD dwStyle;
    DWORD dwExStyle = WS_EX_APPWINDOW;

    if ( app->FullScreen() )
    {
        dwStyle	= WS_POPUP;								
        ShowCursor( FALSE );								
    }
    else if ( app->ResizeAllowed() )
    {
        dwStyle	= WS_OVERLAPPEDWINDOW;
    }
	else if(app->MultipleMonitors())
    {
		dwStyle = WS_POPUP | WS_VISIBLE | WS_SYSMENU;
		dwExStyle = 0;
    }else
    {
        // This removes WS_THICKFRAME and WS_MAXIMIZEBOX, both of which allow
        // resizing of windows.
        dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    }

    RECT rect = { 0, 0, app->Width(), app->Height() };
    AdjustWindowRectEx( &rect, dwStyle, FALSE, dwExStyle );

    // Create the application window.
    std::wstring wtitle;
    std::string title = app->Title(); 
    wtitle.assign( title.begin(), title.end() );

    HWND handle = NULL;

    handle = CreateWindowEx     (	dwExStyle, 
                                    sWindowClass,
                                    wtitle.c_str(),
                                    dwStyle , 
                                    app->XPos(),
                                    app->YPos(),
                                    rect.right - rect.left,
                                    rect.bottom - rect.top, 
                                    0, 
                                    0, 
                                    GHInstance,
                                    0 );
    if( !handle )
    {
        MessageBox( NULL, L"Failed to create application window.", L"Black Vision Application Error", MB_OK | MB_ICONEXCLAMATION );
        return NULL;			
    }

    return handle;
}

// *********************************
//
void DestroyApplicationWindow( WindowedApplication * app, HWND handle )
{
    if ( app->FullScreen() )
    {
        ChangeDisplaySettings	( NULL, 0 );				
        ShowCursor				( TRUE );							
    }

    if ( !GWindowDestroyed )
    {
        if ( !DestroyWindow( handle ) )			
        {
            MessageBox( NULL, L"Could Not Release window handle.",L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }
    }

    if ( !UnregisterClass( sWindowClass, GHInstance ) )		
    {
        MessageBox( NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
    }
}

} // anonymous

// *********************************
//
int WindowedApplication::MainFun	( int argc, char ** argv )
{
    if( Initialize( argc, argv ) )
    {
        bool quit = false;
        while( !quit )
        {
            quit = MainLoopStep();
        }
    }

    TerminateStep();
    return 0;
}

// ***********************
//
bool            WindowedApplication::Initialize     ( int argc, char ** argv )
{
    //#ifdef _DEBUG
    //	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    //#endif

    { argc; argv; } // FIXME: suppress unused warning
    WindowedApplication * app = static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance );

    //FIXME: implement
    //Camera::SetDefaultDepthType(Camera::PM_DEPTH_MINUS_ONE_TO_ONE);

    if( !app->OnPrecreate() )
    {
        return false;
    }

    handle = NULL;
    handle = CreateApplicationWindow( app );
    if( !handle )
    {
        return false;
    }

    app->SetWindowId( ( INT )( INT_PTR )( handle ) );

    /*RendererInput ri;
    ri.m_WindowHandle			= handle;
    ri.m_PixelFormat			= 0;
    ri.m_RendererDC				= 0;
    ri.m_DisableVerticalSync	= true;*/

    RendererInput ri = app->GetRendererInput();
    ri.m_WindowHandle = handle;

    assert( !m_Renderer );
    m_Renderer = new bv::Renderer( ri, app->Width(), app->Height() );

    assert( !m_audioRenderer );
    m_audioRenderer = new audio::AudioRenderer();

    if( app->OnInitialize() )
    {
        GLUtils::DumpGLInfo();
        GLUtils::DumpGLInfoCaps();

        app->OnPreidle();

        ShowWindow	( handle, SW_SHOW );
        UpdateWindow( handle );

        app->OnIdle();

        app->OnPreMainLoop();

        return true;
    }

    return false;
}

// ***********************
//
bool            WindowedApplication::MainLoopStep   ()
{
    WindowedApplication * app = static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance );

    bool quit = false;

    // Eat all events from windows message loop and call OnIdle one time.
    while( !quit )
    {
        MSG msg;
        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
            {
                quit = true;
                return quit;
            }

            HACCEL accel = 0;
            if( !TranslateAccelerator( handle, accel, &msg ) )
            {
                TranslateMessage( &msg );
                DispatchMessage	( &msg );
            }
        }
        else
        {
            // Returns false if application wants to quit itself.
            quit = !app->OnIdle();
            return quit;
        }
    }

    return quit;
}

// ***********************
//
void            WindowedApplication::TerminateStep  ()
{
    WindowedApplication * app = static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance );

    app->OnTerminate();

    delete m_Renderer;
    delete m_audioRenderer;

    DestroyApplicationWindow( app, handle );
}


} //bv
