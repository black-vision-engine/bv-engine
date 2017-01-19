#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class OutputVideo : public OutputInstance
{
private:

    MemoryChunkPtr  m_audioData;

    unsigned int    m_fps;
	bool			m_lastFrameHasAudio;

public:

                            OutputVideo     ( unsigned int width, unsigned int height );

    virtual void            ProcessFrameData( NRenderContext * ctx, RenderResult * result ) override;

private:

    videocards::AVFramePtr  PrepareAVFrame  ( audio::AudioRenderer * audio, Texture2DPtr videoFrame );

};

} //nrl
} //bv
