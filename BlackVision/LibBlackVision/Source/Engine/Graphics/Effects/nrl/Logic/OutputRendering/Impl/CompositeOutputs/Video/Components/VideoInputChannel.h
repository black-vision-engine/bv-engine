#pragma once

#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"


namespace bv { namespace nrl {

//////////////////////////////////////
// Wrapper for render channels - keeps track of readback textures
class VideoInputChannel
{
protected:

    const RenderChannel *   m_wrappedRenderChannel;
    
    bool                    m_lastFrameHadAudio;

protected:

                                VideoInputChannel           ( const RenderChannel * wrappedChannel );

public:
   
    virtual                     ~VideoInputChannel          ();

    bool                        IsActive                    () const;

    bool                        LastFrameHadAudio           () const;
    void                        SetLastFrameHadAudio        ( bool flagValue );

    virtual Texture2DPtr        ReadColorTexture            ( NRenderContext * ctx ) const;
    virtual void                InvalidateCachedTexture     () const;

    static VideoInputChannel *  Create                      ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height );

};

} //bv
} //nrl
