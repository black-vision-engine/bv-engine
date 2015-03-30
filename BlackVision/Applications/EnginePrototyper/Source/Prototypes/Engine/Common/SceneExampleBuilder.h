#pragma once

#include <vector>

namespace bv {

class SceneNode;


// FIXME: later on add tools to create specified geometry presets ( rect, cube, circle, cylinder and so on and move it to the BlackVision application tools)
class SceneExampleBuilder
{
public:

    static SceneNode *          BuildScene              ( unsigned int sceneNum );

private:

    static SceneNode *          Scene0                  ();
    static SceneNode *          Scene1                  ();
    static SceneNode *          Scene2                  ();

};

} //bv
