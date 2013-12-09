#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/WindowedApplication.h"
#include "Engine/Models/Plugins/Simple/SimpleTextPlugin.h"


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

public:

    static void			StaticInitializer	();
    static bool			RegisterInitializer	();
    static bool			m_sWindowedApplicationInitialized;

public:

    BlackVisionApp		        ();
    ~BlackVisionApp		        ();

    virtual void OnKey          ( unsigned char c );
    virtual void OnIdle         ();
    virtual bool OnInitialize   ();
    virtual void OnTerminate    ();

private:

    void    InitializeConsole   ();
    void    InitializeAppLogic  ();
    void    InitializeReadback  ();

    //FIXME: hack to be removed
    void    ReadBackFrameBuffer ();

};

}