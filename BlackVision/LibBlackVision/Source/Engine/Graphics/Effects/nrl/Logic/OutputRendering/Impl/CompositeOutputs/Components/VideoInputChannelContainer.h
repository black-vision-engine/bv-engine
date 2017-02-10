#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/RenderedChannelsDataDesc.h"


namespace bv { namespace nrl {

class VideoInputChannelsContainer
{
private:

//    std::vector< RenderChannel * >  m_renderChannels;

private:

    //FIXME: use the 'delete' keyword
    VideoInputChannelsContainer                ( const VideoInputChannelsContainer & src );
    VideoInputChannelsContainer &  operator =  ( VideoInputChannelsContainer & src );

private:

                            VideoInputChannelsContainer    ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );

public:


                            ~VideoInputChannelsContainer   ();

    const RenderChannel *   GetRenderChannel        ( RenderChannelType rct ) const;    
    void                    InvalidateCachedTexture ( RenderChannelType rct );

    Texture2DPtr            ReadColorTexture        ( Renderer * renderer, RenderChannelType rct );

    void                    UpdateRenderChannels    ();

    bool                    IsActive                ( RenderChannelType rct ) const;
    void                    SetIsActive             ( RenderChannelType rct, bool isActive );

    bool                    ContainsValidData       ( RenderChannelType rct ) const;
    void                    SetContainsValidData    ( RenderChannelType rct, bool containsValidData );
  
public:

    static VideoInputChannelsContainer *   Create   ( const RenderedChannelsDataDesc & desc, RenderTargetStackAllocator * allocator ); 

};

} //nrl
} //bv
