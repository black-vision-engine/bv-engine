#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffect.h"


namespace bv { namespace nrl {

//////////////////////////////////////
// Wrapper for render channels - keeps track of readback textures
class VideoInputChannel
{
private:
    
    unsigned int            m_width;
    unsigned int            m_height;

    RenderChannel *         m_renderChannel;

    mutable Texture2DPtr    m_cachedReadbackTexture;
    mutable bool            m_cachedReadbackUpToDate;

    NFullscreenEffect *     m_blitEffect;

    bool                    m_containsValidData;

private:

                            VideoInputChannel           ( RenderChannel * wrappedChannel );

public:
                            ~VideoInputChannel          ();

    bool                    ContainsValidData           () const;
    void                    SetContainsValidData        ( bool isDataValid );

    Texture2DPtr            ReadColorTexture            ( Renderer * renderer ) const;
    void                    InvalidateCachedTexture     () const;

};

} //bv
} //nrl
