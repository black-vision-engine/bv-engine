#pragma once

#include <vector>
#include "Engine/Graphics/Types/Transform.h"


namespace bv {

class SceneNode;


// FIXME: later on add tools to create specified geometry presets ( rect, cube, circle, cylinder and so on and move it to the BlackVision application tools)
class SceneExampleBuilder
{
public:

    static SceneNode *          BuildScene              ( unsigned int sceneNum );

    static void                 SetLocalTransform       ( SceneNode * node, const Transform & transform );
    static void                 SetWorldTransform       ( SceneNode * node, const Transform & transform );

private:

    static SceneNode *          Scene0                  ();
    static SceneNode *          Scene1                  ();
    static SceneNode *          Scene2                  ();

};

} //bv
