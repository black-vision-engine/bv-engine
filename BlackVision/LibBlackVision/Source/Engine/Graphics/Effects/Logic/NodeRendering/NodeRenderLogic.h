#pragma once

namespace bv {

class Renderer;
class Scene;
class SceneNode;
class SceneNodeRepr;
class RenderTarget;

namespace audio {

class AudioRenderer;

}



class RenderContext;

// FIXME: nrl - this static class is starting to get a bit messy
// FIXME: nrl - think of splitting it into smaller and more manageable functional parts
class NodeRenderLogic
{
public:

	// PUBLIC API
    static void     Clear               ( const RenderTarget * rt, RenderContext * ctx, const glm::vec4 & color = glm::vec4(0.f) );
    
    static void     RenderAudio         ( Scene * scene, RenderContext * ctx );
    static void     RenderAudio         ( SceneNode * node, audio::AudioRenderer * renderer );

    static void     RenderQueued        ( Scene * scene, const RenderTarget * output, RenderContext * ctx );
    static void     RenderQueued        ( SceneNode * node, RenderContext * ctx );

    static void     Render              ( SceneNode * node, RenderContext * ctx );
    static void     Render              ( SceneNode * node, const RenderTarget * output, RenderContext * ctx );

private:

    static void     RenderImpl          ( SceneNode * node, RenderContext * ctx );
    static void     RenderGridLines     ( Scene * scene, RenderContext * ctx );

public:

	// PUBLIC API
    static void     Render              ( SceneNodeRepr * nodeRepr, RenderContext * ctx );
    static void     Render              ( SceneNodeRepr * nodeRepr, const RenderTarget * output, RenderContext * ctx );

    static void     RenderQueued        ( SceneNodeRepr * nodeRepr, const RenderTarget * output, RenderContext * ctx );
    static void     RenderQueued        ( SceneNodeRepr * nodeRepr, RenderContext * ctx );

private:

    static void     RenderImpl          ( SceneNodeRepr * nodeRepr, RenderContext * ctx );

public:

	// PUBLIC API
    static void     RenderRoot          ( SceneNodeRepr * nodeRepr, RenderContext * ctx );
    static void     RenderChildren      ( SceneNodeRepr * nodeRepr, RenderContext * ctx, unsigned int firstChildIdx = 0 );

    static void     RenderBoundingBox   ( SceneNode * node, RenderContext * ctx );

};


} // bv
