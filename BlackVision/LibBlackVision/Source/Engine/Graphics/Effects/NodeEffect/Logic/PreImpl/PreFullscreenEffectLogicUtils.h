#pragma once

#include "Mathematics/glm_inc.h"

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

    static void         RenderSceneNodeToRenderTarget   ( SceneNode * node, RenderLogicContext * ctx, RenderTarget * outputs );

    static glm::vec2    ScreenPosToFullScreenTexPos     ( const glm::vec3 & screenPos, bool cameraPerspective );
};

} // bv