#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Output.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"


namespace bv { namespace nrl {

class OutputStreamSharedMem : public Output
{
private:
    /*
    <PhysicalOutputs>
        <Preview width="640" height="360" renderChannel="RenderChannel_1" windowType="FULLSCREEN" top="0" left="0" /> <!-- WINDOWED | FRAMELESS | FULLSCREEN -->
        <Videocard name="BlueFish">
            <DeviceID value="1" />
            <Channels>
                <Channel name="A" renderer="1" renderChannel="RenderChannel_1" link="HD_1">
                    <Output type="FILL_KEY" resolution="HD" refresh="5000" interlaced="true" flipped="true" referenceMode="FREERUN" referenceH="0" referenceV="0" />
                </Channel>
                <Channel name="C" renderer="1" renderChannel="RenderChannel_2" link="SD_1">
                    <Output type="KEY" resolution="SD_16_9" refresh="5000" interlaced="true" flipped="true" referenceMode="FREERUN" referenceH="0" referenceV="0" />
                </Channel>
            </Channels>
        </Videocard>
        <Stream type="SharedMemory" name="bv_1" namespace="bv" width="640" height="360" renderChannel="RenderChannel_1" />
    </PhysicalOutputs>
    */
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
