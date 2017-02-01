#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

// FIXME: nrl - implement (if necessary) additional logic to cache readback textures - or move this logic somewhere else
class RenderedChannelsData
{
private:

    std::vector< RenderChannel * >  m_renderChannels;
    std::vector< bool >             m_containsValidData;

    std::vector< Texture2DPtr >     m_cachedReadbackTextures;
    std::vector< bool >             m_cachedReadbackUpToDate;

private:

    //FIXME: use the 'delete' keyword
    RenderedChannelsData                ( const RenderedChannelsData & src );
    RenderedChannelsData &  operator =  ( RenderedChannelsData & src );

public:

                            RenderedChannelsData    ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );
                            ~RenderedChannelsData   ();

    const RenderChannel *   GetRenderChannel        ( RenderChannelType rct ) const;
	const RenderTarget *	GetActiveRenderTarget	( RenderChannelType rct ) const;
    
    void                    InvalidateCachedTexture ( RenderChannelType rct );
    Texture2DPtr            ReadColorTexture        ( Renderer * renderer, RenderChannelType rct );

    void                    UpdateRenderChannels    ();

    bool                    IsActive                ( RenderChannelType rct ) const;
    void                    SetIsActive             ( RenderChannelType rct, bool isActive );

    bool                    ContainsValidData       ( RenderChannelType rct ) const;
    void                    SetContainsValidData    ( RenderChannelType rct, bool containsValidData );
    
};

} //nrl
} //bv
