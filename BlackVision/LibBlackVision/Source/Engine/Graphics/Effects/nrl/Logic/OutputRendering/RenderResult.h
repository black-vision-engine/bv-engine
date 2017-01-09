#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/Scene.h"


namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

enum class RenderOutputChannelType : unsigned int
{
    ROCT_OUTPUT_1 = 0,
    ROCT_OUTPUT_2,
    ROCT_OUTPUT_3,
    ROCT_OUTPUT_4,
    ROCT_OUTPUT_5,

    ROCT_TOTAL
};

class RenderOutputChannel;

class RenderResult
{
private:

    std::vector< RenderOutputChannel * >    m_renderOutputChannels;

private:

    //FIXME: use the 'delete' keyword
    RenderResult                ( const RenderResult & src );
    RenderResult &  operator =  ( RenderResult & src );

public:

    RenderResult                                            ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType );
    ~RenderResult                                           ();

    const RenderOutputChannel *     GetRenderOutputChannel  ( RenderOutputChannelType roct ) const;
	const RenderTarget *			GetActiveRenderTarget	( RenderOutputChannelType roct ) const;

    void                            UpdateOutputChannels    ( const SceneVec & scenes );

};

} //nrl
} //bv
