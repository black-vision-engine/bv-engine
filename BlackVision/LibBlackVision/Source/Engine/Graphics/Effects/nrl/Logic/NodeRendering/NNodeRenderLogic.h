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

namespace nrl {

class NRenderContext;

// FIXME: nrl - this static class is starting to get a bit messy
// FIXME: nrl - think of splitting it into smaller and more manageable functional parts
class NNodeRenderLogic
{
public:

	// PUBLIC API
    static void     Clear               ( const RenderTarget * rt, NRenderContext * ctx, const glm::vec4 & color = glm::vec4(0.f) );
    
    static void     RenderAudio         ( Scene * scene, NRenderContext * ctx, std::set< const audio::AudioEntity * > & audioEntities );
    static void     RenderAudio         ( SceneNode * node, audio::AudioRenderer * renderer, std::set< const audio::AudioEntity * > & audioEntities );

    static void     RenderQueued        ( Scene * scene, const RenderTarget * output, NRenderContext * ctx );
    static void     RenderQueued        ( SceneNode * node, NRenderContext * ctx );

    static void     Render              ( SceneNode * node, NRenderContext * ctx );
    static void     Render              ( SceneNode * node, const RenderTarget * output, NRenderContext * ctx );

private:

    static void     RenderImpl          ( SceneNode * node, NRenderContext * ctx );
    static void     RenderGridLines     ( Scene * scene, NRenderContext * ctx );

public:

	// PUBLIC API
    static void     Render              ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );
    static void     Render              ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx );

    static void     RenderQueued        ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx );
    static void     RenderQueued        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );

private:

    static void     RenderImpl          ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );

public:

	// PUBLIC API
    static void     RenderRoot          ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );
    static void     RenderChildren      ( SceneNodeRepr * nodeRepr, NRenderContext * ctx, unsigned int firstChildIdx = 0 );

    static void     RenderBoundingBox   ( SceneNode * node, NRenderContext * ctx );

};

} // nrl
} // bv
