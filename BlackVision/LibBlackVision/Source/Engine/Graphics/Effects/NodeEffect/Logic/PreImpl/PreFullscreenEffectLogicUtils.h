#pragma once

#include "Mathematics/glm_inc.h"
#include "Mathematics/Box.h"

namespace bv
{

class SceneNode;
class RenderLogicContext;
class RenderTarget;
class TransformableEntity;
class Camera;

class PFLogicUtils 
{
public:

    static void                         RenderSceneNodeToRenderTarget   ( SceneNode * node, RenderLogicContext * ctx, RenderTarget * outputs );
    static void                         CalcCommonBoxForNode            ( SceneNode * node, mathematics::Box * box );
};

} // bv