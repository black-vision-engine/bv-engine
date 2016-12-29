#pragma once

#include <vector>

namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

enum class RenderOutputChannelType : unsigned int
{
    ROCT_FIRST_OUTPUT = 0,
    ROCT_SECOND_OUTPUT,

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

    void                            UpdateOutputChannels    ();

};

} //nrl
} //bv
