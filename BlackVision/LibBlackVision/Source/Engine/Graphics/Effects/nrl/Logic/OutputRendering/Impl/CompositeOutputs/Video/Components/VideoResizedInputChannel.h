#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Components/VideoInputChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffect.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { namespace nrl {

//////////////////////////////////////
// Wrapper for render channels - keeps track of readback textures
class VideoResizedInputChannel : public VideoInputChannel
{
private:

    unsigned int            m_width;
    unsigned int            m_height;

    mutable NRenderedData   m_activeRenderOutput;

    mutable RenderTarget *  m_videoRT;
    mutable Texture2DPtr    m_cachedReadbackTexture;
    mutable bool            m_cachedReadbackUpToDate;

    NFullscreenEffect *     m_blitEffect;

public:

                                VideoResizedInputChannel    ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height );
                                ~VideoResizedInputChannel   ();

    virtual Texture2DPtr        ReadColorTexture            ( NRenderContext * ctx ) const override;
    virtual void                InvalidateCachedTexture     () const override;

};

} //bv
} //nrl
