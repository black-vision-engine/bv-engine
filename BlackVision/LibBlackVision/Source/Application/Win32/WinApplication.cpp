#include "stdafx.h"

#include "Application/WindowedApplication.h"

#include "System/InitSubsystem.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"


LRESULT CALLBACK DefaultWindowEventHandler ( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam );

namespace bv {

namespace {

    HINSTANCE GHInstance;

    bool GWindowDestroyed			= false;
    const int GNumPelBits			= 32;
    const wchar_t sWindowClass[]	= L"Black Vision Window App - OpenGL";

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
            app->OnResize( w, h );
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

    RECT rect = { 0, 0, app->Width()-1, app->Height() - 1 };
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
                                    rect.right - rect.left + 1,
                                    rect.bottom - rect.top + 1, 
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
    { argc; argv; } // FIXME: suppress unused warning
    WindowedApplication * app = static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance );

    //FIXME: implement
    //Camera::SetDefaultDepthType(Camera::PM_DEPTH_MINUS_ONE_TO_ONE);

    if ( !app->OnPrecreate() )
    {
        return -1;
    }

    handle = NULL;
    handle = CreateApplicationWindow( app );
    if (!handle )
    {
        return -1;
    }

    app->SetWindowId( (INT)(INT_PTR)( handle ) );

    /*RendererInput ri;
    ri.m_WindowHandle			= handle;
    ri.m_PixelFormat			= 0;
    ri.m_RendererDC				= 0;
    ri.m_DisableVerticalSync	= true;*/

	RendererInput ri = app->GetRendererInput();
	ri.m_WindowHandle = handle;
    
    assert( !m_Renderer );
    m_Renderer = new bv::Renderer( ri, app->Width(), app->Height() );

    if ( app->OnInitialize() )
    {
        GLUtils::DumpGLInfo();
        GLUtils::DumpGLInfoCaps();

        app->OnPreidle();

        ShowWindow	( handle, SW_SHOW );
        UpdateWindow( handle );

        app->OnIdle();

        app->OnPreMainLoop();

        bool quit = false;
        while ( !quit )
        {
            MSG msg;
            if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
            {
                if ( msg.message == WM_QUIT )
                {
                    quit = true;
                    continue;
                }

                HACCEL accel = 0;
                if ( !TranslateAccelerator( handle, accel, &msg ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage	( &msg );
                }
            }
            else
            {
                app->OnIdle();
            }
        }
    }

    app->OnTerminate();

    delete m_Renderer;

    DestroyApplicationWindow( app, handle );

    return 0;
}

} //bv
