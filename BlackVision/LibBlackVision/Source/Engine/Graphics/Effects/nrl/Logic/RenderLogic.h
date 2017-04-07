#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/RenderLogicDesc.h"


namespace bv { 

class Renderer;

namespace audio {

class AudioRenderer;

}

namespace nrl {

class OutputLogic;
class RenderedChannelsData;

class RenderLogic
{
public:

    virtual                         ~RenderLogic           ();

    virtual void                    HandleFrame             ( Renderer * renderer, audio::AudioRenderer * audio, const SceneVec & scenes ) = 0;

    // FIXME: this may or may not be necessary as this interface is onlu used to set OutputLogic and RenderChannelsData state - for which there may be another means
    virtual OutputLogic *           GetOutputLogic          () = 0;
    virtual RenderedChannelsData *  GetRenderedChannelsData () = 0;

public:

    static RenderLogic *           Create                  ( RenderLogicDesc & desc );

};

} // nrl
} // bv
