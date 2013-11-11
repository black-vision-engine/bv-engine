#pragma once

#include "Engine/Graphics/SceneGraph/IScene.h"


namespace bv {

class GlobalState
{
private:

    IScene* scene;
    float time;
    
    GlobalState(IScene* scene);

public:
    void evaluate();

    static GlobalState* createGlobalState(IScene* scene);
};

IScene* createTestScene();


} //bv
