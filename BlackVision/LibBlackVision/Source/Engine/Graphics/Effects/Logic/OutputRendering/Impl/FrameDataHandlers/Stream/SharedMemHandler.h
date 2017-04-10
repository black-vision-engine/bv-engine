#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffect.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"


namespace bv { namespace nrl {
   
class SharedMemHandler : public FrameDataHandler
{
private:

    unsigned int                m_width;
    unsigned int                m_height;

    RenderedData               m_activeRenderOutput;

    FullscreenEffect *         m_mixChannelsEffect;

    RenderTarget *              m_shmRT;
    Texture2DPtr                m_shmTexture;

    SharedMemoryVideoBufferPtr  m_shmVideoBuffer;

public:

                                                SharedMemHandler    ( unsigned int width, unsigned int height );
                                                ~SharedMemHandler   ();

    virtual void                                HandleFrameData     ( const OutputState & state, RenderContext * ctx, const RenderChannel * channel ) override;

    virtual FullscreenEffectComponentStatePtr  GetInternalFSEState () override;

private:

    Texture2DPtr    PrepareFrame            ( const OutputState & state, RenderContext * ctx, const RenderChannel * inputChannel );

    void            ProcessFrame            ( Texture2DPtr frame );

    Texture2DPtr    ReadDefaultTexture      ( RenderContext * ctx, const RenderChannel * inputChannel );
    Texture2DPtr    ReadMixChannelsTexture  ( RenderContext * ctx, const RenderTarget * inputRenderTarget );

};

} //nrl
} //bv
