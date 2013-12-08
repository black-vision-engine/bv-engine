#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Application/WindowedApplication.h"
#include "Engine/Events/Events.h"
#include "Engine/Models/Plugins/Simple/SimpleTextPlugin.h"
#include "MockFrameReader.h"

namespace bv {

namespace model
{
    class BasicNode;
    class ModelScene;
}

class SceneNode;
class Camera;
class MockFrameReader;

class BlackVisionApp : public WindowedApplication
{
private:

    model::ModelScene *         m_modelScene;
    SceneNode *                 m_mockSceneEng;

    std::ofstream               m_file; // For debugging

    FrameRenderedEventPtr       m_frameRenderedEvent;
    model::SetTextEventPtr      m_frameSetTextEvent;
    MockFrameReader *           m_mockFrameReader;

    char *                      m_frameData;

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

    void    AddCameraAnimation  ();
    void    AddCameraAnimation2 ();

    bool    RenderScene         ();
    bool    RenderNode          ( SceneNode * node );

    void    InitializeConsole   ();
    void    InitializeModelScene();
    void    InitializeReadback  ();

    //FIXME: hack to be removed
    void    ReadBackFrameBuffer ();

};

}