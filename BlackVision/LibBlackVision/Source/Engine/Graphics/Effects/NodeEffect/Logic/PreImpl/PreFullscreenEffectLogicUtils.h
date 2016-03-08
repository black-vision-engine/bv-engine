#pragma once

namespace bv
{

class SceneNode;
class RenderLogicContext;
class RenderTarget;

class PFLogicUtils 
{
public:

    static void RenderSceneNodeToRenderTarget( SceneNode * node, RenderLogicContext * ctx, RenderTarget * outputs );
};

} // bv