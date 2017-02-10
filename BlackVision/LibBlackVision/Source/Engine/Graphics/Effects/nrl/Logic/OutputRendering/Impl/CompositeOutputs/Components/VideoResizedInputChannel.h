#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffect.h"


namespace bv { namespace nrl {

//////////////////////////////////////
// Wrapper for render channels - keeps track of readback textures
class VideoInputChannel
{
private:

    const RenderChannel *   m_renderChannel;

    mutable Texture2DPtr    m_cachedReadbackTexture;
    mutable bool            m_cachedReadbackUpToDate;

    NFullscreenEffect *     m_blitEffect;

private:

                                VideoInputChannel           ( const RenderChannel * wrappedChannel );

public:
                                ~VideoInputChannel          ();

    virtual Texture2DPtr        ReadColorTexture            ( Renderer * renderer ) const;
    virtual void                InvalidateCachedTexture     () const;

    static VideoInputChannel *  Create                      ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height );

};

} //bv
} //nrl
