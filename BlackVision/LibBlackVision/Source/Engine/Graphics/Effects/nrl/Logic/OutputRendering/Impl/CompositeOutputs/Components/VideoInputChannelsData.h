#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Components/VideoInputChannel.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { namespace nrl {

// FIXME: nrl - implement (if necessary) additional logic to cache readback textures - or move this logic somewhere else
class VideoInputChannelsData
{
private:

    std::vector< VideoInputChannel * >  m_renderChannels;

private:

    //FIXME: use the 'delete' keyword
    VideoInputChannelsData                ( const VideoInputChannelsData & src );
    VideoInputChannelsData &  operator =  ( VideoInputChannelsData & src );

private:

                            VideoInputChannelsData    ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );

public:


                            ~VideoInputChannelsData   ();

    const VideoInputChannel *   GetInputChannel         ( RenderChannelType rct ) const;

    void                        InvalidateCachedTextres ();
    void                        InvalidateCachedTexture ( RenderChannelType rct );
    Texture2DPtr                ReadColorTexture        ( Renderer * renderer, RenderChannelType rct );

    bool                        IsActive                ( RenderChannelType rct ) const;
    bool                        ContainsValidData       ( RenderChannelType rct ) const;
  
public:

    static VideoInputChannelsData *   Create          ( const RenderedChannelsDataDesc & desc, RenderTargetStackAllocator * allocator ); 

};

} //nrl
} //bv
