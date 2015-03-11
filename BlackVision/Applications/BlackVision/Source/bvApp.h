#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/WindowedApplication.h"
#include "Tools/SimpleTimer.h"


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

    virtual void    OnKey               ( unsigned char c ) override;
    virtual void    OnPreidle	        () override;
    virtual void    OnIdle              () override;
    virtual void    OnPreMainLoop       () override;
    virtual bool    OnInitialize        () override;
    virtual void    OnTerminate         () override;

private:

    void            UpdateSubsystems    ( unsigned long millis );

    void            InitializeConsole   ();
    void            InitializeAppLogic  ();
    void            InitializeSelfState ();
	void			InitializeResourceLoaders ();

    void            PostFrame           ( unsigned int millis );
};

} //bv
