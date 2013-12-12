#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/WindowedApplication.h"
#include "System/SimpleTimer.h"


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

public:

    static bool			m_sWindowedApplicationInitialized;

public:

    static void			StaticInitializer	();
    static bool			RegisterInitializer ();

public:

                    BlackVisionApp		();
                    ~BlackVisionApp	    ();

    virtual void    OnKey               ( unsigned char c );
    virtual void    OnPreidle	        ();
    virtual void    OnIdle              ();
    virtual void    OnPreMainLoop       ();
    virtual bool    OnInitialize        ();
    virtual void    OnTerminate         ();

private:

    void            UpdateSubsystems    ( unsigned long millis );

    void            InitializeConsole   ();
    void            InitializeAppLogic  ();

};

} //bv
