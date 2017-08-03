#pragma once

namespace bv {

class Renderer;
class Scene;
class SceneNode;
class SceneNodeRepr;
class RenderTarget;




class RenderContext;

// FIXME: nrl - this static class is starting to get a bit messy
// FIXME: nrl - think of splitting it into smaller and more manageable functional parts
class DepthRenderLogic
{
public:

    // PUBLIC API
    static void     RenderQueued        ( Scene * scene, const RenderTarget * output, RenderContext * ctx );
    static void     RenderQueued        ( SceneNode * node, RenderContext * ctx );

    static void     Render              ( SceneNode * node, RenderContext * ctx );

private:

    static void     RenderImpl          ( SceneNode * node, RenderContext * ctx );
};


} // bv
