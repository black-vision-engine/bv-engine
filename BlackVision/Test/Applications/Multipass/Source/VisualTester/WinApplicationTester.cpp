//#include "WinApplicationTester.h"
//#include "System/InitSubsystem.h"
//#include "Engine/Graphics/Renderers/Renderer.h"
//#include "Engine/Graphics/SceneGraph/Camera.h"
//#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"
//#include "BVConfig.h"
//
//
//
//const int defaultWindowWidth = 700;
//const int defaultWindowHeight = 700;
//const bool fullscreenMode = false;
//
// 
//
//LRESULT CALLBACK DefaultWindowEventHandler ( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam );
//
//namespace bv
//{
//
//WinApplicationTester* application = nullptr;
//
//
//
//// *********************************
//// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
//void			WinApplicationTester::MainInitializer	()
//{
//	bv::ApplicationBase::MainFun = &bv::WindowedApplication::MainImpl;
//    bv::ApplicationBase::ApplicationInstance = new WinApplicationTester();
//}
//
//
//// *********************************
//// FIXME: move it to valid BV windowed version of engine and wrap with a macro
//bool			WinApplicationTester::RegisterInitializer	()
//{
//    bv::InitSubsystem::AddInitializer( WinApplicationTester::MainInitializer );
//
//    return true;
//}
//
//bool WinApplicationTester::m_sWindowedApplicationInitialized = WinApplicationTester::RegisterInitializer();
//
//
//
//WinApplicationTester::WinApplicationTester()
//	:	WindowedApplication( "Rendering test", 0, 0, DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight(),bv::WindowMode::WINDOWED,*(new bv::RendererInput) )
//{
//	application = this;
//}
//
//
//WinApplicationTester::~WinApplicationTester(void)
//{
//	if( m_renderLogic )
//		delete m_renderLogic;
//}
//
//void WinApplicationTester::InitCamera         ( unsigned int w, unsigned int h )
//{
//    Camera * cam = new bv::Camera();
//
//    cam->SetFrame( DefaultConfig.CameraPosition(), DefaultConfig.CameraDirection(), DefaultConfig.CameraUp() );
//    
//    if( cam->IsPerspective() )
//    {
//        cam->SetPerspective( DefaultConfig.FOV(), w, h, DefaultConfig.NearClippingPlane(), DefaultConfig.FarClippingPlane() );
//    }
//    else
//    {
//        cam->SetViewportSize( w, h );
//    }
//
//    m_Renderer->SetCamera( cam );
//    m_renderLogic->SetCamera( cam );
//
//    //FIXME: read from configuration file and change the camera appropriately when current resoultion changes
//}
//
//
//bool WinApplicationTester::OnInitialize()
//{
//	m_renderLogic = new VisualTesterRenderLogic();
//
//	InitCamera( DefaultConfig.DefaultwindowWidth(), DefaultConfig.DefaultWindowHeight() );
//
//	return WindowedApplication::OnInitialize();
//}
//
///**@brief Function used to test rendering. It compares reference image with image rendered from
//SceneNode given in parameter. If images are diffrent, function creates image that shows the difference.
//
//If you want to render refenrence image, set parameter makeReferenceImage to true, and specify
//fileName, where you want to place it. Reference image has name:
//fileName + "RereferenceImage" + file_ext.
//When specifying name by testing, you shuldn't give a full name, but only the first part.
//
//@param[in] fileName Reference image to compare (or create depending on parameter makeReferenceImage). See description above, how to make a name.
//@param[in] node Scene node to render.
//@param[in] makeReferenceImage By setting true you can render reference image instead of making test. Default: false.*/
//void WinApplicationTester::testRender( const std::string fileName, SceneNode* node, bool makeReferenceImage )
//{
//	if( makeReferenceImage )
//		m_renderLogic->renderReferenceImage( m_Renderer, node, fileName );
//	else
//		m_renderLogic->renderCompareWithReferenceImage( m_Renderer, node, fileName );
//}
//
//
//Renderer* WinApplicationTester::getRenderer()
//{
//	return m_Renderer;
//}
//
//
//namespace {
//
//    HINSTANCE GHInstance;
//
//    bool GWindowDestroyed			= false;
//    const int GNumPelBits			= 32;
//    const wchar_t sWindowClass[]	= L"Black Vision Window App - OpenGL";
//
//}
//
//// *********************************
//// TODO: implement more handlers if required (and add them to windowed application callbacks interface)
//LRESULT CALLBACK DefaultWindowEventHandler ( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
//{
//    WindowedApplication * app = static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance );
//
//    if ( !app || !app->WindowId() )
//    {
//        return DefWindowProc( handle, msg, wParam, lParam );
//    }
//
//    switch ( msg ) 
//    {
//        case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            BeginPaint( handle, &ps );
//            app->OnDisplay();
//            EndPaint( handle, &ps );
//            return 0;
//        }
//        case WM_ERASEBKGND:
//        {
//            // This tells Windows not to erase the background (and that the
//            // application is doing so).
//            return 1;
//        }
//        case WM_MOVE:
//        {
//            int xPos = (int) ( LOWORD(lParam) );
//            int yPos = (int) ( HIWORD(lParam) );
//            app->OnMove( xPos, yPos );
//            return 0;
//        }
//        case WM_SIZE:
//        {
//            int w = (int)(LOWORD(lParam));
//            int h = (int)(HIWORD(lParam));
//            app->OnResize( w, h );
//            return 0;
//        }
//        case WM_CHAR:
//        {
//            unsigned char key = (unsigned char)(char)wParam;
//
//            //FIXME: possibly use other key
//            if ( key == VK_ESCAPE )
//            {
//                PostQuitMessage(0);
//                return 0;
//            }
//
//            app->OnKey( key );
//
//            return 0;
//        }
//        case WM_DESTROY:
//        {
//            GWindowDestroyed = true;
//            PostQuitMessage( 0 );
//            return 0;
//        }
//    }
//
//    return DefWindowProc( handle, msg, wParam, lParam );
//}
//
//
//namespace {
//
//// *********************************
////
//HWND CreateApplicationWindow ( WindowedApplication * app )
//{
//    WNDCLASS wc;
//
//    GHInstance			= GetModuleHandle( NULL );
//    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//    wc.lpfnWndProc		= DefaultWindowEventHandler;
//    wc.cbClsExtra		= 0;
//    wc.cbWndExtra		= 0;
//    wc.hInstance		= GHInstance;
//    wc.hIcon			= LoadIcon(GHInstance, IDI_APPLICATION);
//    wc.hCursor			= LoadCursor(0, IDC_ARROW);
//    wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
//    wc.lpszClassName	= sWindowClass;
//    wc.lpszMenuName		= 0;
//  
//    if ( !RegisterClass( &wc ) )
//    {
//        MessageBox( NULL, L"Failed to register the window class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
//        return NULL;										
//    }
//
//
//    if ( app->FullScreen() )
//    {
//        DEVMODE dmScreenSettings;								
//
//        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );	
//        
//        dmScreenSettings.dmSize			= sizeof( dmScreenSettings );		
//        dmScreenSettings.dmPelsWidth	= app->Width();				
//        dmScreenSettings.dmPelsHeight	= app->Height();				
//        dmScreenSettings.dmBitsPerPel	= GNumPelBits;					
//        dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
//
//        
//        if ( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
//        {
//            
//            if ( MessageBox( NULL, L"Fulscreen mode not available. Switch to windowed version?", sWindowClass, MB_YESNO | MB_ICONEXCLAMATION ) == IDYES )
//            {
//                app->UnsetFulscreen();
//            }
//            else
//            {            
//                return NULL;							
//            }
//        }
//    }
//
//    DWORD dwStyle;
//    DWORD dwExStyle = WS_EX_APPWINDOW;
//
//    if ( app->FullScreen() )
//    {
//        dwStyle	= WS_POPUP;								
//        ShowCursor( FALSE );								
//    }
//    else if ( app->ResizeAllowed() )
//    {
//        dwStyle	= WS_OVERLAPPEDWINDOW;
//    }
//    else
//    {
//        // This removes WS_THICKFRAME and WS_MAXIMIZEBOX, both of which allow
//        // resizing of windows.
//        dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
//    }
//
//    RECT rect = { 0, 0, app->Width()-1, app->Height() - 1 };
//    AdjustWindowRectEx( &rect, dwStyle, FALSE, dwExStyle );
//
//    // Create the application window.
//    std::wstring wtitle;
//    std::string title = app->Title(); 
//    wtitle.assign( title.begin(), title.end() );
//
//    HWND handle = NULL;
//
//    handle = CreateWindowEx     (	dwExStyle, 
//                                    sWindowClass,
//                                    wtitle.c_str(),
//                                    dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
//                                    app->XPos(),
//                                    app->YPos(),
//                                    rect.right - rect.left + 1,
//                                    rect.bottom - rect.top + 1, 
//                                    0, 
//                                    0, 
//                                    GHInstance,
//                                    0 );
//    if( !handle )
//    {
//        MessageBox( NULL, L"Failed to create application window.", L"Black Vision Application Error", MB_OK | MB_ICONEXCLAMATION );
//        return NULL;			
//    }
//
//    return handle;
//}
//
//// *********************************
////
//void DestroyApplicationWindow( WindowedApplication * app, HWND handle )
//{
//    if ( app->FullScreen() )
//    {
//        ChangeDisplaySettings	( NULL, 0 );				
//        ShowCursor				( TRUE );							
//    }
//
//    if ( !GWindowDestroyed )
//    {
//        if ( !DestroyWindow( handle ) )			
//        {
//            MessageBox( NULL, L"Could Not Release window handle.",L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
//        }
//    }
//
//    if ( !UnregisterClass( sWindowClass, GHInstance ) )		
//    {
//        MessageBox( NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
//    }
//}
//
//}
//
//
//
//
//
//
//int WinApplicationTester::MainFun( int argc, char ** argv )
//{
//	::testing::InitGoogleTest( &argc, argv );		// Change comparing to normal WindowedApplication::MainFun
//	argc;	argv;
//
//    WinApplicationTester * app = static_cast< WinApplicationTester * >( ApplicationBase::ApplicationInstance );
//
//    //FIXME: implement
//    //Camera::SetDefaultDepthType(Camera::PM_DEPTH_MINUS_ONE_TO_ONE);
//
//    if ( !app->OnPrecreate() )
//    {
//        return -1;
//    }
//
//    handle = NULL;
//    handle = CreateApplicationWindow( app );
//    if (!handle )
//    {
//        return -1;
//    }
//
//    app->SetWindowId( (INT)(INT_PTR)( handle ) );
//
//    RendererInput ri;
//    ri.m_WindowHandle			= handle;
//    ri.m_PixelFormat			= 0;
//    ri.m_RendererDC				= 0;
//    ri.m_DisableVerticalSync	= true;
//    
//    assert( !m_Renderer );
//    m_Renderer = new bv::Renderer( ri, app->Width(), app->Height() );
//
//
//
//    if ( app->OnInitialize() )
//    {
//        GLUtils::DumpGLInfo();
//        GLUtils::DumpGLInfoCaps();
//
//        app->OnPreidle();
//
//		// W zasadzie nie musimy pokazywaæ okna, bo i tak do niego nie renderujemy.
//        //ShowWindow	( handle, SW_SHOW );
//        //UpdateWindow( handle );
//
//        //app->OnIdle();
//
//
//        app->OnPreMainLoop();
//
//        bool quit = false;
//        while ( !quit )
//        {
//            MSG msg;
//            if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
//            {
//                if ( msg.message == WM_QUIT )
//                {
//                    quit = true;
//                    continue;
//                }
//
//                HACCEL accel = 0;
//                if ( !TranslateAccelerator( handle, accel, &msg ) )
//                {
//                    TranslateMessage( &msg );
//                    DispatchMessage	( &msg );
//                }
//            }
//            else
//            {
//				// Change comparing to normal WindowedApplication::MainFun
//                app->OnIdle();		// TESTS
//				quit = true;		// TESTS are invoked only once
//            }
//        }
//    }
//
//    app->OnTerminate();
//
//    delete m_Renderer;
//
//    DestroyApplicationWindow( app, handle );
//	application = nullptr;
//
//	return 0;
//}
//
//
//void WinApplicationTester::OnIdle()
//{
//	RUN_ALL_TESTS();
//}
//
////
////// *********************************
////// Entry point for all applications derived from this class
////int main( int argc, char * argv[] )
////{
////	::testing::InitGoogleTest( &argc, argv );
////
//////TODO: Initialize here all generic subsystems here (loggers, generic managers, system wide subsystems)
//////TODO: Register (and/or) initialize all (possibly already registered) components
//////TODO: Load all generic resources
////
////    bv::InitSubsystem::CallInitializers();
////
////    assert( bv::ApplicationBase::ApplicationInstance != nullptr );
////
////    //FIXME: Why not use polymorphism here??
////    int exitCode = bv::ApplicationBase::MainFun( argc, argv );
////
//////TODO: deinitialize all subsystems
//////TODO: Free all generic resources
//////TODO: make sure that all logs are closed and no data is corrupted and/or saved 
////
////    //FIXME: this should be done in some kind of termination subsystem
////    delete bv::ApplicationBase::ApplicationInstance;
////
////	RUN_ALL_TESTS();
////
////	return exitCode;
////}
//
//
//}