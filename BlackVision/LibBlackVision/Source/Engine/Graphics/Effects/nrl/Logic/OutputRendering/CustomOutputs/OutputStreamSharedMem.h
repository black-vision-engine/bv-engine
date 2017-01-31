#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Output.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"


namespace bv { namespace nrl {

class OutputStreamSharedMem : public Output
{
private:

    NRenderedData               m_activeRenderOutput;

    NFullscreenEffect *         m_mixChannelsEffect;

    RenderTarget *              m_shmRT;
    Texture2DPtr                m_shmTexture;

    SharedMemoryVideoBufferPtr  m_shmVideoBuffer;

public:

                    OutputStreamSharedMem   ( unsigned int width, unsigned int height );
                    ~OutputStreamSharedMem  ();

    virtual void    ProcessFrameData        ( NRenderContext * ctx, RenderResult * input ) override;

private:

    void            UpdateEffectValues      ();

    Texture2DPtr    PrepareFrame            ( NRenderContext * ctx, RenderResult * input );
    void            ProcessFrame            ( Texture2DPtr frame );

    Texture2DPtr    ReadDefaultTexture      ( NRenderContext * ctx, RenderResult * input, RenderChannelType rct );
    Texture2DPtr    ReadMixChannelsTexture  ( NRenderContext * ctx, const RenderTarget * inputRenderTarget );

};

} //nrl
} //bv
