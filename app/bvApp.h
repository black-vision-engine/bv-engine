#pragma once

#include "WindowedApplication.h"
#include <fstream>

namespace bv {

class BasicNode;
class SceneNode;
class Camera;
class ModelScene;

class BlackVisionApp : public WindowedApplication
{
private:
    ModelScene*         m_modelScene;
    SceneNode*          m_mockSceneEng;

    std::ofstream       m_file; // For debugging

public:

    static void			StaticInitializer	();
    static bool			RegisterInitializer	();
    static bool			m_sWindowedApplicationInitialized;

public:

    BlackVisionApp		        ();

    virtual void OnIdle         ();
    virtual bool OnInitialize   ();
    virtual void OnTerminate    ();

private:

    void    AddCameraAnimation  ();
	void    AddCameraAnimation2 ();

    bool    RenderScene         ();
    bool    RenderNode          ( SceneNode* node);

};

}