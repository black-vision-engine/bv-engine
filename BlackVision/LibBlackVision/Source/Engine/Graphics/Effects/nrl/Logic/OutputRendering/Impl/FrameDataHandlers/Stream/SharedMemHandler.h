#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffect.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"


namespace bv { namespace nrl {
   
class SharedMemHandler : public FrameDataHandler
{
private:

    unsigned int                m_width;
    unsigned int                m_height;

    NRenderedData               m_activeRenderOutput;

    NFullscreenEffect *         m_mixChannelsEffect;

    RenderTarget *              m_shmRT;
    Texture2DPtr                m_shmTexture;

    SharedMemoryVideoBufferPtr  m_shmVideoBuffer;

public:

                                                SharedMemHandler    ( unsigned int width, unsigned int height );
                                                ~SharedMemHandler   ();

    virtual void                                HandleFrameData     ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * channel ) override;

    virtual NFullscreenEffectComponentStatePtr  GetInternalFSEState () override;

private:

    Texture2DPtr    PrepareFrame            ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * inputChannel );

    void            ProcessFrame            ( Texture2DPtr frame );

    Texture2DPtr    ReadDefaultTexture      ( NRenderContext * ctx, const RenderChannel * inputChannel );
    Texture2DPtr    ReadMixChannelsTexture  ( NRenderContext * ctx, const RenderTarget * inputRenderTarget );

};

} //nrl
} //bv
