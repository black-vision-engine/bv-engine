#pragma once

#include "Prototypes/Engine/Common/ShaderDataSourceCreator.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class SceneNode;

class SceneExampleBuilder
{
public:

    static SceneNode *          BuildScene              ( unsigned int sceneNum );

private:

    static SceneNode *          Scene0                  ();

private:

    static RenderableEffectPtr  CreateRenderableEffect  ( ShaderDataSourceType sdst );

};

} //bv
