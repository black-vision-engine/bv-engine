#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class OutputVideo : public OutputInstance
{
private:

    MemoryChunkPtr  m_audioData;

    // FIXME: nrl - what about this one? How should it be cofigured? Ask Pawelek.
    unsigned int    m_fps;

	bool			m_lastFrameHasAudio;

    RenderTarget *  m_videoRT;
    Texture2DPtr    m_videoTexture;

public:

                            OutputVideo     ( unsigned int width, unsigned int height );
                            ~OutputVideo    ();

    virtual void            ProcessFrameData( NRenderContext * ctx, RenderResult * result ) override;

private:

    videocards::AVFramePtr  PrepareAVFrame  ( audio::AudioRenderer * audio, Texture2DPtr videoFrame );

};

} //nrl
} //bv
