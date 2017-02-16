#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/VideoTypes.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { namespace nrl {

// FIXME: nrl - implement (if necessary) additional logic to cache readback textures - or move this logic somewhere else
class VideoInputChannelsData
{
private:

    VideoInputChannelsMap               m_outputToChannelsMapping;
    VideoInputChannelsVec               m_videoInputChannels;

    const RenderedChannelsData *        m_originalRenderedChannelsData;

    bool                                m_initialized;

private:

    //FIXME: use the 'delete' keyword
    VideoInputChannelsData                ( const VideoInputChannelsData & src );
    VideoInputChannelsData &  operator =  ( VideoInputChannelsData & src );

private:

                                VideoInputChannelsData  ();

public:


                                ~VideoInputChannelsData ();

    bool                        IsInitialized           () const;
    bool                        Initialize              ( const RenderedChannelsData * rcd ) const;

    const VideoInputChannel *   GetInputChannel         ( unsigned int videoOutputID ) const;

    void                        InvalidateCachedTextures();
    void                        InvalidateCachedTexture ( RenderChannelType rct );
    Texture2DPtr                ReadColorTexture        ( NRenderContext * ctx, RenderChannelType rct ) const;

    bool                        IsActive                ( RenderChannelType rct ) const;
    bool                        ContainsValidData       ( RenderChannelType rct ) const;
  
public:

    static VideoInputChannelsData *   Create            ( const RenderedChannelsData * renderedChannelsData ); 

};

} //nrl
} //bv
