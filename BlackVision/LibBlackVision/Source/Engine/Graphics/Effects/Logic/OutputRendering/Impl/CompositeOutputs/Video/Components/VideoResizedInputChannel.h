#pragma once

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"
#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffect.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"


namespace bv { namespace nrl {

//////////////////////////////////////
// Wrapper for render channels - keeps track of readback textures
class VideoResizedInputChannel : public VideoInputChannel
{
private:

    unsigned int            m_width;
    unsigned int            m_height;

    mutable RenderedData   m_activeRenderOutput;

    mutable RenderTarget *  m_videoRT;
    mutable Texture2DPtr    m_cachedReadbackTexture;
    mutable bool            m_cachedReadbackUpToDate;

    FullscreenEffect *     m_blitEffect;

public:

                                VideoResizedInputChannel    ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height );
                                ~VideoResizedInputChannel   ();

    virtual Texture2DPtr        ReadColorTexture            ( RenderContext * ctx ) const override;
    virtual void                InvalidateCachedTexture     () const override;

};

} //bv
} //nrl
