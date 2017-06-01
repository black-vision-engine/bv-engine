#pragma once

#include <vector>

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"

namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;



// FIXME: nrl - implement (if necessary) additional logic to cache readback textures - or move this logic somewhere else
class RenderedChannelsData
{
private:

    std::vector< RenderChannel * >  m_renderChannels;
    std::vector< bool >             m_containsValidData;

private:

    //FIXME: use the 'delete' keyword
    RenderedChannelsData                ( const RenderedChannelsData & src );
    RenderedChannelsData &  operator =  ( RenderedChannelsData & src );

private:

                            RenderedChannelsData    ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );

public:


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
  
    bool                    ContainsValidData       ( const RenderChannel * channel ) const;

public:

    static RenderedChannelsData *   Create          ( const RenderedChannelsDataDesc & desc, RenderTargetStackAllocator * allocator ); 

};


} //bv
