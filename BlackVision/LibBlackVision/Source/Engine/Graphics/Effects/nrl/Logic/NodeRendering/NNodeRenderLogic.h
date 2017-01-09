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

class NNodeRenderLogic
{
public:

	// PUBLIC API
    static void     RenderAudio         ( Scene * scene, NRenderContext * ctx );
    static void     RenderAudio         ( SceneNode * node, audio::AudioRenderer * renderer );

    static void     RenderQueued        ( Scene * scene, const RenderTarget * output, NRenderContext * ctx );
    static void     RenderQueued        ( SceneNode * node, const RenderTarget * output, NRenderContext * ctx );

    static void     Render              ( SceneNode * node, NRenderContext * ctx );
    static void     Render              ( SceneNode * node, const RenderTarget * output, NRenderContext * ctx );

private:

    static void     RenderImpl          ( SceneNode * node, NRenderContext * ctx );

public:

	// PUBLIC API
    static void     Render              ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );
    static void     Render              ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx );

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
