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

    const RenderedChannelsData *        m_wrappedRenderedChannelsData;

    std::vector< VideoInputChannel * >  m_renderChannels;

    VideoInputChannelsMap               m_videoOutputRenderChannels;

private:

    //FIXME: use the 'delete' keyword
    VideoInputChannelsData                ( const VideoInputChannelsData & src );
    VideoInputChannelsData &  operator =  ( VideoInputChannelsData & src );

private:

                                VideoInputChannelsData  ( const RenderedChannelsData * renderedChannelsData );

public:


                                ~VideoInputChannelsData ();

    const VideoInputChannel *   GetInputChannel         ( RenderChannelType rct ) const;

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
