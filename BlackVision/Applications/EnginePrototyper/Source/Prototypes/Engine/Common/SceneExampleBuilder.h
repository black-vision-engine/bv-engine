#pragma once

#include <vector>

#include "Prototypes/Engine/Common/Transformations.h"


namespace bv {

class SceneNode;


// FIXME: later on add tools to create specified geometry presets ( rect, cube, circle, cylinder and so on and move it to the BlackVision application tools)
class SceneExampleBuilder
{
public:

    static SceneNode *          BuildScene              ( unsigned int sceneNum );
    static void                 UpdateScene             ( unsigned int sceneNum, SceneNode * root, float t );
    static void                 OnKeyScene              ( unsigned int sceneNum, SceneNode * root, unsigned char c );

    static void                 SetLocalTransform       ( SceneNode * node, const Transform & transform );
    static void                 SetWorldTransform       ( SceneNode * node, const Transform & transform );
    static void                 SetPixelShaderParam     ( SceneNode * node, const std::string & name, const glm::vec4 & param );

private:

    static SceneNode *          Scene0                  ();
    static SceneNode *          Scene1                  ();
    static SceneNode *          Scene2                  ();

    static void                 UpdateScene0            ( SceneNode * root, float t );
    static void                 UpdateScene1            ( SceneNode * root, float t );
    static void                 UpdateScene2            ( SceneNode * root, float t );

    static void                 OnKeyScene0             ( SceneNode * root, unsigned char c );
    static void                 OnKeyScene1             ( SceneNode * root, unsigned char c );
    static void                 OnKeyScene2             ( SceneNode * root, unsigned char c );

};

} //bv
