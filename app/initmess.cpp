/********************************************************************	
    created:	18:11:2012   17:16
    author:		Radek Larry Grabinski
    email:		radekgrabinski@gmail.com
    
    project:	BlackVision
    desc:		Main App Init & Loop
*********************************************************************/
#define GLEW_STATIC
#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/gl.h>		
#include <gl/glu.h>	
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"

#include "BasicTestScene.h"

using namespace std; 


HDC			hDC=NULL;		
HGLRC		hRC=NULL;		
HWND		hWnd=NULL;		
HINSTANCE	hInstance;	
MMRESULT	TimerID;
MMRESULT	TimerTLO;

bool	done=FALSE;	
bool	keys[256];			
bool	active=TRUE;	
bool	fullscreen = false;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)	
{
    if (height==0)									
    {
        height=1;									
    }

    glViewport(0,0,width,height);					

    glMatrixMode(GL_PROJECTION);					
    glLoadIdentity();								

    gluPerspective(45.0f,(GLfloat)width/((GLfloat)height),0.2, 50000.0);

    glEnable(GL_CULL_FACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMateriali(GL_FRONT,GL_SHININESS,128);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_MODELVIEW);	
    glClearDepth(20.0f);	

    glEnable(GL_COLOR_MATERIAL);
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_TEXTURE_2D);

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);						
    glLoadIdentity();								
}

int InitGL(GLvoid)									
{
    //glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			
    glClearDepth(20.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);								
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glEnable(GL_COLOR_MATERIAL);
    //glPolygonMode(GL_FRONT, GL_FILL);
    //glEnable(GL_TEXTURE_2D);

    return TRUE;									
}

//int DrawGLScene(GLvoid)								
//{
//    engine->Render();
//    return TRUE;									
//}

void InitGlew()
{
    glewInit();
        
    if (!GLEW_ARB_vertex_program || !glewGetExtension("GL_ARB_fragment_program")) 
    {		
        MessageBox(NULL,L"No support for ARB vertex'n'fragment program!\nI can't work this way!", L"BlackVision", MB_OK|MB_ICONASTERISK);
        exit(0);
    }
}

GLvoid KillGLWindow(GLvoid)							
{
    if (fullscreen)
    {
        ChangeDisplaySettings(NULL,0);				
        ShowCursor(TRUE);							
    }

    if (hRC)										
    {
        if (!wglMakeCurrent(NULL,NULL))
        {
            MessageBox(NULL,L"Release Of DC And RC Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC))					
        {
            MessageBox(NULL,L"Release Rendering Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        hRC=NULL;									
    }

    if (hDC && !ReleaseDC(hWnd,hDC))				
    {
        MessageBox(NULL,L"Release Device Context Failed.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDC=NULL;									
    }

    if (hWnd && !DestroyWindow(hWnd))				
    {
        MessageBox(NULL,L"Could Not Release hWnd.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd=NULL;									
    }

    if (!UnregisterClass(L"OpenGL",hInstance))		
    {
        MessageBox(NULL,L"Could Not Unregister Class.",L"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;								
    }
}



/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(LPCWSTR title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint		PixelFormat;		
    WNDCLASS	wc;					
    DWORD		dwExStyle;
    DWORD		dwStyle;			
    RECT		WindowRect;			
    WindowRect.left=(long)0;			
    WindowRect.right=(long)width;	
    WindowRect.top=(long)0;			
    WindowRect.bottom=(long)height;	

    fullscreen=fullscreenflag;		

    hInstance			= GetModuleHandle(NULL);			
    wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
    wc.lpfnWndProc		= (WNDPROC) WndProc;					
    wc.cbClsExtra		= 0;									
    wc.cbWndExtra		= 0;									
    wc.hInstance		= hInstance;							
    wc.hIcon			=  LoadIcon(wc.hInstance,IDI_APPLICATION);
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
    wc.hbrBackground	= NULL;									
    wc.lpszMenuName		= NULL;									
    wc.lpszClassName	= L"OpenGL";	
    
    if (!RegisterClass(&wc))								
    {
        MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;										
    }
    
    if (fullscreen)
    {
        DEVMODE dmScreenSettings;								
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
        dmScreenSettings.dmPelsWidth	= width;				
        dmScreenSettings.dmPelsHeight	= height;				
        dmScreenSettings.dmBitsPerPel	= bits;					
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
        {
            
            if (MessageBox(NULL,L"Nie ma fulskrina. W oknie?",L"BlackVision",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
            {
                fullscreen=FALSE;	
            }
            else
            {
            
                MessageBox(NULL,L"To ja znikam.",L"BlackVision",MB_OK|MB_ICONSTOP);
                return FALSE;							
            }
        }
    }

    if (fullscreen)										
    {
        dwExStyle=WS_EX_APPWINDOW;						
        dwStyle=WS_POPUP;								
        ShowCursor(FALSE);								
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle=WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME;			
    }


    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		


    if (!(hWnd=CreateWindowEx(	dwExStyle,						
                                L"OpenGL",						
                                title,							
                                dwStyle |					
                                WS_CLIPSIBLINGS |				
                                WS_CLIPCHILDREN,				
                                0,0,// tu pozycja x,y okna.							
                                WindowRect.right-WindowRect.left,
                                WindowRect.bottom-WindowRect.top,
                                NULL,							
                                NULL,							
                                hInstance,						
                                NULL)))							
    {
        KillGLWindow();		
        MessageBox(NULL,L"Blad utworzenia okna.",L"Buond",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;			
    }

    static	PIXELFORMATDESCRIPTOR pfd=		
	{
        sizeof(PIXELFORMATDESCRIPTOR),			
        1,										
        PFD_DRAW_TO_WINDOW |					
        PFD_SUPPORT_OPENGL |					
        PFD_DOUBLEBUFFER,					
        PFD_TYPE_RGBA,							
        bits,									
        0, 0, 0, 0, 0, 0,						
        0,										
        0,										
        0,										
        0, 0, 0, 0,								
        32,										
        0,									
        0,									
        PFD_MAIN_PLANE,						
        0,									
        0, 0, 0								
    };
    
    if (!(hDC=GetDC(hWnd)))						
    {
        KillGLWindow();						
        MessageBox(NULL,L"#1",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;					
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
    {
        KillGLWindow();								
        MessageBox(NULL,L"#2",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;						
    }

    PixelFormat = ChoosePixelFormat (hDC, &pfd);				// Find A Compatible Pixel Format
        if (PixelFormat == 0)												// Did We Find A Compatible Format?
        {
            // Failed
            ReleaseDC (hWnd, hDC);							// Release Our Device Context
            hDC = 0;												// Zero The Device Context
            DestroyWindow (hWnd);									// Destroy The Window
            hWnd = 0;												// Zero The Window Handle
            return FALSE;													// Return False
        }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd))	
    {
        KillGLWindow();						
        MessageBox(NULL,L"#3",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;						
    }

    if (!(hRC=wglCreateContext(hDC)))			
    {
        KillGLWindow();							
        MessageBox(NULL,L"#4",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;							
    }

    if(!wglMakeCurrent(hDC,hRC))				
    {
        KillGLWindow();						
        MessageBox(NULL,L"#5",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;						
    }

	//!!!UP TO NOW!!!
    ShowWindow(hWnd,SW_SHOW);				
    SetForegroundWindow(hWnd);				
    SetFocus(hWnd);							
    ReSizeGLScene(width, height);			


    if (!InitGL())						
    {
        KillGLWindow();						
        MessageBox(NULL,L"Blad inicjalizacji.",L"Buond",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;						
    }

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    return TRUE;							
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	
                            UINT	uMsg,	
                            WPARAM	wParam,	
                            LPARAM	lParam)	
{
    switch (uMsg)							
    {
        case WM_ACTIVATE:					
        {
            if (!HIWORD(wParam))			
            {								
                active=TRUE;
                
            }
            else
            {
                active=FALSE;					
            }

            
            return 0;						
        }

        case WM_SYSCOMMAND:					
        {
            switch (wParam)					
            {
                case SC_SCREENSAVE:			
                case SC_MONITORPOWER:		
                return 0;					
            }
            break;							
        }

        case WM_CLOSE:						
        {
            PostQuitMessage(0);				
            return 0;						
        }

        case WM_KEYDOWN:					
        {
            switch (wParam)
            {
                case VK_F1:					
                    return 0;
            }

            keys[wParam] = TRUE;				
            return 0;						
        }

        case WM_KEYUP:						
        {
            keys[wParam] = FALSE;			
            return 0;						
        }

        case WM_SIZE:						
        {
            ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));

            return 0;							
        }

        case WM_LBUTTONDOWN:
        {
                    
            return 0;
        }
    

    }

    
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void Swap(void)
{		
    SwapBuffers(hDC);
}


//void main(int argc, TCHAR* argv[], TCHAR* envp[])
//{
//    MSG		msg;														
//
//    CreateGLWindow(L".: BlackVision :.", 1024, 768, 32, fullscreen);
//    InitGlew();	
//
//    GLUtils::dumpGLInfo(); 
//
//    bv::IScene* scene = bv::createTestScene();
//    bv::GlobalState* gs = bv::GlobalState::createGlobalState(scene);
//
//    while(!done)								
//    {
//        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
//        {
//            if (msg.message==WM_QUIT)			
//            {											
//                done=TRUE;						
//            }
//            else								
//            {
//                TranslateMessage(&msg);			
//                DispatchMessage(&msg);			
//            }
//        }
//        else									
//        {
//            
//            if (active)						
//            {
//                if (keys[VK_ESCAPE])			
//                {
//
//                    done=TRUE;					
//                }
//                else							
//                {
//                    gs->evaluate();
//                    //DrawGLScene();						
//                    SwapBuffers(hDC);			
//                }
//            }
//        
//        }
//    }
//    KillGLWindow();	
//    //return (msg.wParam);					
//}

 