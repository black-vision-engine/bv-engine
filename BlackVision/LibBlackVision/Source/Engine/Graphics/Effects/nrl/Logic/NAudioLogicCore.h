#pragma once


namespace bv { 

class SceneNode;

namespace audio {

class AudioRenderer;

}

namespace nrl {

class NRenderContext;

class NAudioLogicCore
{
public:

    void                    RenderAudio          ( SceneNode * root, NRenderContext * ctx );

private:

    void                    RenderAudioImpl      ( SceneNode * node, audio::AudioRenderer * renderer );

};

} //nrl
} //bv
