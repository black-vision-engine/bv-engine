#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderChannel.h"


namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

class RenderResult
{
private:

    std::vector< RenderChannel * >  m_renderChannels;
    std::vector< bool >             m_containsValidData;

private:

    //FIXME: use the 'delete' keyword
    RenderResult                ( const RenderResult & src );
    RenderResult &  operator =  ( RenderResult & src );

public:

                            RenderResult            ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );
                            ~RenderResult           ();

    const RenderChannel *   GetRenderChannel        ( RenderChannelType rct ) const;
	const RenderTarget *	GetActiveRenderTarget	( RenderChannelType rct ) const;

    void                    UpdateRenderChannels    ();

    bool                    IsActive                ( RenderChannelType rct ) const;
    void                    SetIsActive             ( RenderChannelType rct, bool isActive );

    bool                    ContainsValidData       ( RenderChannelType rct ) const;
    void                    SetContainsValidData    ( RenderChannelType rct, bool containsValidData );
    
};

} //nrl
} //bv
