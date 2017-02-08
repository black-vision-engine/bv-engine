#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

using videocards::AVFramePtr;

class MockVideo : public OutputInstance
{
private:

    NRenderedData   m_activeRenderOutput;

    MemoryChunkPtr  m_audioData;

    // FIXME: nrl - what about this one? How should it be cofigured? Ask Pawelek.
    unsigned int    m_fps;

	bool			m_lastFrameHasAudio;

    NFullscreenEffect * m_mixChannelsEffect;

    RenderTarget *      m_videoRT;
    Texture2DPtr        m_videoTexture;

public:

                    OutputVideo             ( unsigned int width, unsigned int height );
                    ~OutputVideo            ();

    virtual void    ProcessFrameData        ( NRenderContext * ctx, RenderResult * input ) override;

private:

    void            UpdateEffectValues      ();

    Texture2DPtr    PrepareFrame            ( NRenderContext * ctx, RenderResult * input );
    void            ProcessFrame            ( NRenderContext * ctx, Texture2DPtr frame );

    Texture2DPtr    ReadDefaultTexture      ( NRenderContext * ctx, RenderResult * input, RenderChannelType rct );
    Texture2DPtr    ReadMixChannelsTexture  ( NRenderContext * ctx, const RenderTarget * inputRenderTarget );

    AVFramePtr      PrepareAVFrame          ( audio::AudioRenderer * audio, Texture2DPtr videoFrame );

};

} //nrl
} //bv
