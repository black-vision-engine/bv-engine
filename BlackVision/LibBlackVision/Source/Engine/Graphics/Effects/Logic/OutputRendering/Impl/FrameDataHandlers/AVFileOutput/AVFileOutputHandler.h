#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffect.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"

#include "Memory/AVFrame.h"

namespace bv { 

namespace avencoder
{
class AVEncoder;
}

namespace audio
{
class AudioMixer;
}
   
class AVFileOutputHandler : public FrameDataHandler
{
private:

    unsigned int                m_width;
    unsigned int                m_height;

    Int32                       m_fps;

    UInt64                      m_lastFrameTime;

    AVFramePtr                  m_avFrame;

    avencoder::AVEncoder *      m_encoder;

    RenderedData                m_activeRenderOutput;

    RenderTarget *              m_shmRT;
    Texture2DPtr                m_shmTexture;

    audio::AudioMixer *         m_audioMixer;

public:

                                                AVFileOutputHandler    ( unsigned int width, unsigned int height );
                                                ~AVFileOutputHandler   ();

    virtual void                                HandleFrameData         ( const OutputState & state, RenderContext * ctx, const RenderChannel * channel ) override;

    virtual FullscreenEffectComponentStatePtr   GetInternalFSEState     () override;

    void                                        StartToAVFileRendering  ( const std::string & outputFilePath );
    void                                        StopToAVFileRendering   ();

private:

    void            PrepareFrame            ( const OutputState & state, RenderContext * ctx, const RenderChannel * inputChannel );

    void            ProcessFrame            ();

    Texture2DPtr    ReadDefaultTexture      ( RenderContext * ctx, const RenderChannel * inputChannel );
};


} //bv
