#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffect.h"

#include "AVFrame.h"

namespace bv { 

namespace audio {

class AudioRenderer;

} // audio

namespace nrl {

class RenderResult;
class NRenderContext;

using videocards::AVFramePtr;

class MockVideoHandler : public FrameDataHandler
{
private:

    unsigned int        m_width;
    unsigned int        m_height;

    NRenderedData       m_activeRenderOutput;

    MemoryChunkPtr      m_audioData;

    // FIXME: nrl - what about this one? How should it be cofigured? Ask Pawelek.
    unsigned int        m_fps;

	bool			    m_lastFrameHasAudio;

    NFullscreenEffect * m_mixChannelsEffect;

    RenderTarget *      m_videoRT;
    Texture2DPtr        m_videoTexture;

public:

                    MockVideoHandler        ( unsigned int width, unsigned int height );
                    ~MockVideoHandler       ();

    virtual void    HandleFrameData         ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * inputChannel ) override;

    virtual NFullscreenEffectComponentStatePtr  GetInternalFSEState () override;

private:

    Texture2DPtr    PrepareFrame            ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * inputChannel );
    void            ProcessFrame            ( NRenderContext * ctx, Texture2DPtr frame );

    Texture2DPtr    ReadDefaultTexture      ( NRenderContext * ctx, const RenderChannel * inputChannel );
    Texture2DPtr    ReadMixChannelsTexture  ( NRenderContext * ctx, const RenderTarget * inputRenderTarget );

    AVFramePtr      PrepareAVFrame          ( audio::AudioRenderer * audio, Texture2DPtr videoFrame );

};

} //nrl
} //bv
