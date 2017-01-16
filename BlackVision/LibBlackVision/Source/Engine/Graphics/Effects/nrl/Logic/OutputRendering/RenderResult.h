#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"


namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

class RenderResult
{
private:

    std::vector< RenderOutputChannel * >    m_renderOutputChannels;
    std::vector< bool >                     m_containsValidData;

private:

    //FIXME: use the 'delete' keyword
    RenderResult                ( const RenderResult & src );
    RenderResult &  operator =  ( RenderResult & src );

public:

                                    RenderResult            ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );
                                    ~RenderResult           ();

    const RenderOutputChannel *     GetRenderOutputChannel  ( RenderOutputChannelType roct ) const;
	const RenderTarget *			GetActiveRenderTarget	( RenderOutputChannelType roct ) const;

    void                            UpdateOutputChannels    ();

    bool                            IsActive                ( RenderOutputChannelType roct ) const;
    void                            SetIsActive             ( RenderOutputChannelType roct, bool isActive );

    bool                            ContainsValidData       ( RenderOutputChannelType roct ) const;
    void                            SetContainsValidData    ( RenderOutputChannelType roct, bool containsValidData );
    
};

} //nrl
} //bv
