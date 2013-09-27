#include "BasicTestScene.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
using std::ifstream;
#include <sstream>
#include <string>
using std::string;

#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"
#include "Engine/Graphics/Renderers/OGLRenderer/gl_core_4_3.h"
#include "Engine/Graphics/SceneGraph/BasicScene.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"

#include <Windows.h>


namespace bv {

GlobalState::GlobalState(IScene* scene)
    : time(0.f), scene(scene)
{
    assert(scene);
}

void GlobalState::evaluate()
{
    static DWORD timeStart = GetTickCount();
    static DWORD frames = 0;
    if((frames++ % 500) == 0)
    {
        DWORD milis = GetTickCount() - timeStart;
        std::cout << "Frames per sec: " << float(frames) / milis * 1000.f << std::endl;
    }

    scene->update(time);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);
    scene->render(nullptr); // TODO:
    time = (float)(GetTickCount() - timeStart) / 1000.f;
}

GlobalState* GlobalState::createGlobalState(IScene* scene)
{
    return new GlobalState(scene);
}

IScene* createTestScene()
{
//    // SOLID
//    Vec4Interpolator color;
//
//    color.addKey(0.f, glm::vec4(0,0,0,0));
//
//    color.addKey(.25f, glm::vec4(1,0.4,0.5,1));
//    color.addKey(.5f, glm::vec4(0.3,0.4,0.5,1));
//    color.addKey(.75f, glm::vec4(0.1,0.2,0.8,1));
//
//    color.addKey(1.f, glm::vec4(1,0.4,0.5,1));
//
//    color.setWrapMethod(WrapMethod::clamp, WrapMethod::pingPong);
//
//    SolidColorPlugin* solid1 = new SolidColorPlugin(color);
//
//    BasicNode* root = new BasicNode();
//
//    root->setPixelShaderPlugin(solid1);
//
//
//    // TRANSFORMATION
//
//    TransformF* t = new TransformF(); 
//
//    FloatInterpolator angle; angle.setWrapPostMethod(WrapMethod::pingPong);
//    FloatInterpolator x;
//    FloatInterpolator y;
//    FloatInterpolator z;
//
//    x.addKey(0.f, 0.f);
//    y.addKey(0.f, 0.f);
//    z.addKey(0.f, -1.f);
//    angle.addKey(0.f, 0.f);
//    angle.addKey(5.f, 180.f);
//
//    t->addRotation(angle, x,y,z);
//
//    model::SimpleTransformChannel * stch = new model::SimpleTransformChannel();
//    stch->AddTransformChannel( t );
//    //Transformation* trans = new Transformation("MVP", *t);
//
//    //FIXME: here
//    //root->AddPlugin( stch );
////    root->addTransformPlugin(trans);
//
//
//    // SCENE BUILDING AND INIT
//
//    BasicScene* scene = new BasicScene();
//
//    SceneNode* rootSceneNode = root->BuildScene();
//
//    scene->setTreeSceneRoot(rootSceneNode);
//
//    scene->initScene();
//
//    Camera* cam = new Camera();
//
//    scene->setCamera(cam);

    return nullptr;
}

} //bv
