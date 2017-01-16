#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"

namespace bv { 

class Renderer;
class SceneNode;

namespace audio {

class AudioRenderer;

}

namespace nrl {

class OutputLogic;

class NRenderLogic
{
public:

    virtual                 ~NRenderLogic   ();

    virtual void            HandleFrame     ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) = 0;

    // FIXME: temporary - just to make sure that required configurations are implemented in a valid manner
    virtual OutputLogic *   GetOutputLogic  () = 0;

};

} // nrl
} // bv
