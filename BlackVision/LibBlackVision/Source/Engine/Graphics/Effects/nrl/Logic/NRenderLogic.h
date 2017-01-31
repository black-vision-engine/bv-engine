#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"


namespace bv { 

class Renderer;

namespace audio {

class AudioRenderer;

}

namespace nrl {

class OutputLogic;
class RenderedChannelsData;

class NRenderLogic
{
public:

    virtual                         ~NRenderLogic           ();

    virtual void                    HandleFrame             ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) = 0;

    virtual OutputLogic *           GetOutputLogic          () = 0;

    virtual RenderedChannelsData *  GetRenderedChannelsData () = 0;

};

} // nrl
} // bv
