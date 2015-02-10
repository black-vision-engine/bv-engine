#pragma once


namespace bv {

class SceneNode;

class SceneExampleBuilder
{
public:

    static SceneNode *  BuildScene( unsigned int sceneNum );

private:

    static SceneNode *  Scene0();

};

} //bv
