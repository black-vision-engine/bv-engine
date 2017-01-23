#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"


namespace bv { namespace nrl {

class OutputStreamSharedMem : public OutputInstance
{
private:

    NRenderedData               m_activeRenderOutput;

    NFullscreenEffect *         m_mixChannelsEffect;

    // FIXME: nlr - move to result somehow and cache it there
    RenderTarget *              m_shmRT;
    Texture2DPtr                m_shmTexture;

    SharedMemoryVideoBuffer *   m_shmVideoBuffer;

public:

                    OutputStreamSharedMem   ( unsigned int width, unsigned int height );
                    ~OutputStreamSharedMem  ();

    virtual void    ProcessFrameData        ( NRenderContext * ctx, RenderResult * input ) override;

private:

    void            UpdateEffectValues      ();

};

} //nrl
} //bv
