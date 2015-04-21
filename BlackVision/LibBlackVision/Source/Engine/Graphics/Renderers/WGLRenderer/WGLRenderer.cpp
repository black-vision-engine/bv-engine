#include "Engine/Graphics/Renderers/Renderer.h"

#include "BVGL.h"

#include "Engine/Graphics/Renderers/WGLRenderer/WGLRendererData.h"


namespace bv {

namespace {

// *********************************
//
bool	InitializeGLContext( RendererInput & ri, WGLRendererData * data )
{
    PIXELFORMATDESCRIPTOR pfd;
    memset( &pfd, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

    pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    pfd.nVersion = 1;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_GENERIC_ACCELERATED;
    pfd.dwFlags |= PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cAlphaBits = 8; //FIXME: NVIDIA CARDS HACK
    pfd.cStencilBits = 8;

    //FIXME: For multisampling verify that: input.mPixelFormat == 0 ? true : false)
    ri.m_PixelFormat= ChoosePixelFormat( data->m_WindowDC, &pfd );
    if ( ri.m_PixelFormat == 0 )
    {
        MessageBox( NULL, L"Failed to choose Pixel Format.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    BOOL success = SetPixelFormat( data->m_WindowDC, ri.m_PixelFormat, &pfd );
    if (!success)
    {
        MessageBox( NULL, L"Failed to set Pixel Format.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    // Create an OpenGL context.
    HGLRC windowRC = wglCreateContext( data->m_WindowDC );
    if ( !windowRC )
    {
        MessageBox( NULL, L"Failed to create GL Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    // Activate the context.
    success = wglMakeCurrent( data->m_WindowDC, windowRC );
    if (!success)
    {
        MessageBox( NULL, L"wglMakeCurrent failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    data->m_WindowRC = windowRC;

    return true;
}

// *********************************
//
bool	InitializeGL	()
{
    if( !BVGL::InitializeBVGL() )
    {
        MessageBox( NULL, L"No support for ARB vertex'n'fragment program!\n", L"Black Vision App - OpenGL", MB_OK | MB_ICONASTERISK );
    }

    return true;
}

// *********************************
//
bool	InitializeVSync( const RendererInput & ri )
{
    // Load wglSwapIntervalExt
    typedef BOOL (APIENTRY * PFNWGLSWAPINTERVALEXTPROC)(int);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr; 
    wglSwapIntervalEXT = reinterpret_cast< PFNWGLSWAPINTERVALEXTPROC >( wglGetProcAddress( "wglSwapIntervalEXT" ) );

    if ( !wglSwapIntervalEXT )
    {
        MessageBox( NULL, L"Cannot load wglSwapIntervalEXT!\n", L"Black Vision App - OpenGL", MB_OK | MB_ICONASTERISK );
        return false;
    }

    if ( ri.m_DisableVerticalSync )
    {
        wglSwapIntervalEXT( 0 );
    }

    return true;
}

} //anonymous namespace

// *********************************
//
Renderer::Renderer ( RendererInput & ri, int w, int h, TextureFormat colorFormat )
{
    Initialize( w, h, colorFormat );

    WGLRendererData * data = new WGLRendererData();
    m_RendererData = static_cast< RendererData * >( data );

    data->m_WindowHandle	= ri.m_WindowHandle;
    data->m_WindowDC		= ::GetDC( ri.m_WindowHandle );
    
    ri.m_RendererDC			= data->m_WindowDC;

    bool success = InitializeGLContext( ri, data );
    assert( success );

    success = InitializeGL();
    assert( success );

    success = InitializeVSync( ri );
    assert( success );

    if ( !success )
    {
        exit( 1 );
    }

    // Some default GL rendering mode
    data->m_CurrentRS.Initialize( m_defaultStateInstance );
}

// *********************************
//
Renderer::~Renderer ()
{
    //FIXME: remove all PDR resources
    WGLRendererData * data = static_cast< WGLRendererData * >( m_RendererData );

    Terminate();

    if ( data->m_WindowRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( data->m_WindowRC );
    }

    if ( data->m_WindowDC )
    {
        ::ReleaseDC( data->m_WindowHandle, data->m_WindowDC );
    }

    delete m_RendererData;
}

// *********************************
//
void Renderer::DisplayColorBuffer ()
{
    SwapBuffers( static_cast< WGLRendererData * >( m_RendererData )->m_WindowDC );
}

} //bv
