#pragma once

#include <vector>

#include "Prototypes/Engine/Common/ShaderDataSourceCreator.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Types/Transform.h"

namespace bv {

class SceneNode;

typedef std::vector< Transform >    TTransformVec;

// FIXME: later on add tools to create specified geometry presets ( rect, cube, circle, cylinder and so on and move it to the BlackVision application tools)
class SceneExampleBuilder
{
public:

    static SceneNode *          BuildScene              ( unsigned int sceneNum );

private:

    static SceneNode *          Scene0                  ();

private:

    static RenderableEffectPtr  CreateRenderableEffect  ( ShaderDataSourceType sdst );
    static RenderableEntity *   CreateRenderable        ( RenderableEffectPtr effect );
    static TTransformVec        CreateDefaultTransform  ();

};

} //bv
