#include "stdafx.h"

#include "WindowedApplication.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Application/ApplicationContext.h"

namespace bv {

// *********************************
//
WindowedApplication::WindowedApplication			( const char * title, int x, int y, int w, int h, WindowMode windowMode, RendererInput ri )
    : m_WindowId( 0 )
    , m_WindowTitle( title )
    , m_xPos( x )
    , m_yPos( y )
    , m_Width( w )
    , m_Height( h )
    , m_MultipleMonitors(false)
    , m_ClearColor( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) )
    , m_Renderer( nullptr )
{
	if(windowMode==WindowMode::WINDOWED)
        m_AllowResize = true;
    else
        m_AllowResize = false;

    if(windowMode==WindowMode::FULLSCREEN)
        m_FullScreen = true;
    else
        m_FullScreen = false;

    if(windowMode==WindowMode::MULTIPLE_SCREENS)
        m_MultipleMonitors = true;
    else
        m_MultipleMonitors = false;

	m_RendererInput = ri;
}

// *********************************
//
WindowedApplication::~WindowedApplication			()
{
}

// *********************************
//
std::string		WindowedApplication::Title			() const
{
    return m_WindowTitle;
}

// *********************************
//
RendererInput		WindowedApplication::GetRendererInput	() const
{
    return m_RendererInput;
}

// *********************************
//
int		WindowedApplication::XPos					() const
{
    return m_xPos;
}

// *********************************
//
int		WindowedApplication::YPos					() const
{
    return m_yPos;
}

// *********************************
//
int		WindowedApplication::Width					() const
{
    return m_Width;
}

// *********************************
//
int		WindowedApplication::Height					() const
{
    return m_Height;
}

// *********************************
//
bool	WindowedApplication::ResizeAllowed			() const
{
    return m_AllowResize;
}

// *********************************
//
bool	WindowedApplication::MultipleMonitors		() const
{
    return m_MultipleMonitors;
}

// *********************************
//
bool	WindowedApplication::FullScreen				() const
{
    return m_FullScreen;
}

// *********************************
//
void	WindowedApplication::UnsetFulscreen			()
{
    m_FullScreen = false;
}

// *********************************
//
float	WindowedApplication::AspectRatio			() const
{
    float fW = (float)Width();
    float fH = (float)Height();

    return fW/fH;
}

// *********************************
//
int		WindowedApplication::WindowId				() const
{
    return m_WindowId;
}

// *********************************
//
void	WindowedApplication::SetWindowId			( int id )
{
    m_WindowId = id;
}

// *********************************
//
void	WindowedApplication::SetClearColor			( const glm::vec4 & col )
{
    m_ClearColor = col;

    m_Renderer->SetClearColor( col );
}

// *********************************
//
glm::vec4	WindowedApplication::ClearColor			() const
{
    return m_ClearColor;
}

// *********************************
//
const Renderer * WindowedApplication::Renderer		() const
{
    return m_Renderer;
}

// *********************************
//
bool	WindowedApplication::OnInitialize			()
{
    // Renderer is already created (by platform dependent initialization), so it can be used
    m_Renderer->SetClearColor( ClearColor() );

    return true;
}

// *********************************
//
void	WindowedApplication::OnTerminate			()
{
    //To be implemented in derived class
}

// *********************************
//
void	WindowedApplication::OnKey					( unsigned char c )
{
    { c; } // FIXME: suppress unused warning
    //To be implemented in derived class
}

// *********************************
//
void	WindowedApplication::OnMove					( int x, int y )
{
    m_xPos	= x;
    m_yPos	= y;
}

// *********************************
//
void	WindowedApplication::OnResize				( int w, int h )
{
    if ( w > 0 && h > 0 )
    {
        if ( m_Renderer )
        {
            m_Renderer->Resize( w, h );
        }

        m_Width		= w;
        m_Height	= h;
    }
}

// *********************************
//
bool	WindowedApplication::OnPrecreate			()
{
    //To be implemented in derived classes
    return true;
}

// *********************************
//
void	WindowedApplication::OnPreidle				()
{
    m_Renderer->ClearBuffers();
}

// *********************************
//
void	WindowedApplication::OnDisplay				()
{
    //To be implemented in derived classes	
}

// *********************************
//
void	WindowedApplication::OnIdle					()
{
    //To be implemented in derived classes
}

// *********************************
//
void    WindowedApplication::OnPreMainLoop          ()
{
    //To be implemented in derived classes
}

// *********************************
//
int WindowedApplication::MainImpl					( int argc, char ** argv )
{
    return static_cast< WindowedApplication * >( ApplicationBase::ApplicationInstance )->MainFun( argc, argv );
}

} // bv
