#pragma once

#include "CoreDEF.h"
#include "ApplicationBase.h"
#include <string>
#include "Mathematics/glm_inc.h"


#include "Engine\Graphics\Renderers\WGLRenderer\WGLRendererInput.h"

namespace bv
{

class Renderer;

namespace audio {
    class AudioRenderer;
}

//pablito
enum WindowMode {FULLSCREEN=0,WINDOWED=1,MULTIPLE_SCREENS=2};

class WindowedApplication : public ApplicationBase
{
protected:

    int				m_WindowId;

    std::string		m_WindowTitle;
    
    int				m_xPos;
    int				m_yPos;
    int				m_Width;
    int 			m_Height;

	//pablito
	RendererInput	m_RendererInput;

    bool			m_AllowResize;
    bool			m_MultipleMonitors;
    bool			m_FullScreen;

    glm::vec4		m_ClearColor;

    Renderer *		        m_Renderer;
    audio::AudioRenderer *  m_audioRenderer;
	
	bool			m_EnableGLFinish;
	bool			m_EnableGLFlush;
	int				m_VSYNFrames;
	

protected:

    WindowedApplication				( const char * title, int x, int y, int w, int h,  WindowMode windowMode, RendererInput ri );

public:

    virtual ~WindowedApplication	();

    virtual int MainFun				( int argc, char ** argv );

	//pablito
	RendererInput	GetRendererInput() const;

    std::string		Title			() const;
    int				XPos			() const;
    int				YPos			() const;
    int				Width			() const;
    int				Height			() const;
    
    bool			ResizeAllowed	() const;
	bool			MultipleMonitors() const;
    bool			FullScreen		() const;
    void			UnsetFulscreen	();

    float			AspectRatio		() const;

    int				WindowId		() const;
    void			SetWindowId		( int id );
    
    void			SetClearColor	( const glm::vec4 & col );
    glm::vec4		ClearColor		() const;

    const Renderer *             Renderer		() const;
    const audio::AudioRenderer * AudioRenderer	() const;

    virtual bool	OnInitialize	();
    virtual void	OnTerminate		();
    virtual void	OnMove			( int x, int y );
    virtual void	OnKey		    ( unsigned char c );
    virtual void    OnMouse         ( MouseAction action, int posX, int posY );
    virtual void	OnResize		( int w, int h );

    virtual bool	OnPrecreate     ();
    virtual void	OnPreidle	    ();
    virtual void	OnDisplay	    ();
    virtual bool	OnIdle		    ();
    virtual void    OnPreMainLoop   ();

protected:

	HWND			handle;

public:

    bool            Initialize      ( int argc, char ** argv );
    bool            MainLoopStep    ();
    void            TerminateStep   ();
    
protected:
    
    static int MainImpl			( int argc, char ** argv );
    
    //FIXME: move it to the derived class
    // Performance measurements.
    //void ResetTime ();
    //void MeasureTime ();
    //void UpdateFrameCount ();
    //void DrawFrameRate (int x, int y, const Float4& color);

    //double mLastTime, mAccumulatedTime, mFrameRate;
    //int mFrameCount, mAccumulatedFrameCount, mTimer, mMaxTimer;
};

}
