#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/WindowedApplication.h"
#include "Tools/SimpleTimer.h"

//pablito
//#include "SocketWrapper.h"
#include "ConfigManager.h"
#include "structure/AssetManager.h"
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
    ProcessManager *            m_processManager; //FIXME: move to engine object

    SimpleTimer                 m_timer;

    //pablito
    bv::videocards::VideoCardManager m_videoCardManager;
public:

    static bool			m_sWindowedApplicationInitialized;

public:

    static void			StaticInitializer		();
    static void			LoggerInitializer		();
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

private:

    void            UpdateSubsystems			( unsigned long millis );

    void            InitializeConsole			();
    void            InitializeAppLogic			();
    void            InitializeSelfState			();
    // pablito
    bool			InitializeLicenses      ();
    void			InitializeConfig        ();

    void            PostFrame					( unsigned int millis );
};

} //bv
