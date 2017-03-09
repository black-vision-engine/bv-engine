#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/WindowedApplication.h"

//pablito
//#include "SocketWrapper.h"
#include "LicenseManager.h"
#include "VideoCardManager.h"


namespace bv {

namespace model
{
    class BasicNode;
    class ModelScene;
}


class SceneNode;
class Camera;
class MockFrameReader;
class ProcessManager;

class BVAppLogic;

class BlackVisionApp : public WindowedApplication
{
private:

    BVAppLogic *                m_app;

public:

    static bool			m_sWindowedApplicationInitialized;

public:

    static void			StaticInitializer		( int argc, char * argv[] );
    static void			LoggerInitializer		( int argc, char * argv[] );
    static bool			RegisterInitializer		();

public:

                    BlackVisionApp				();
                    ~BlackVisionApp				();

    virtual void    OnKey						( unsigned char c ) override;
    virtual void    OnMouse                     ( MouseAction action, int posX, int posY ) override;
    virtual void    OnPreidle					() override;
    virtual bool    OnIdle						() override;
    virtual void    OnPreMainLoop				() override;
    virtual bool    OnInitialize				() override;
    virtual void    OnTerminate					() override;
    virtual void	OnResize		            ( int w, int h ) override;

private:

    void            InitializeConsole			();
    void            InitializeAppLogic			();
    void            InitializeSelfState			();
    // pablito
    bool			InitializeLicenses          ();

    void            PostFrame                   ();
};

} //bv
