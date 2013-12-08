#pragma once

#include "ApplicationBase.h"
#include <string>
#include <glm/glm.hpp>
#include <Windows.h>

namespace bv
{

class Renderer;

class WindowedApplication : public ApplicationBase
{
protected:

    int				m_WindowId;

    std::string		m_WindowTitle;
    
    int				m_xPos;
    int				m_yPos;
    int				m_Width;
    int 			m_Height;

    bool			m_AllowResize;
    bool			m_FullScreen;

    glm::vec4		m_ClearColor;

    Renderer *		m_Renderer;
	

protected:

    WindowedApplication				( const char * title, int x, int y, int w, int h, bool fullScreen = false );

public:

    virtual ~WindowedApplication	();

    virtual int MainFun				( int argc, char ** argv );

    std::string		Title			() const;
    int				XPos			() const;
    int				YPos			() const;
    int				Width			() const;
    int				Height			() const;
    
    bool			ResizeAllowed	() const;
    bool			FullScreen		() const;
    void			UnsetFulscreen	();

    float			AspectRatio		() const;

    int				WindowId		() const;
    void			SetWindowId		( int id );
    
    void			SetClearColor	( const glm::vec4 & col );
    glm::vec4		ClearColor		() const;

    const Renderer* Renderer		() const;

    virtual bool	OnInitialize	();
    virtual void	OnTerminate		();
    virtual void	OnMove			( int x, int y );
    virtual void	OnKey		    ( unsigned char c );
    virtual void	OnResize		( int w, int h );

    virtual bool	OnPrecreate ();
    virtual void	OnPreidle	();
    virtual void	OnDisplay	();
    virtual void	OnIdle		();

	HWND			handle;
    
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
