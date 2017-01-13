#include "stdafx.h"

#include "RenderResult.h"

#include <cassert>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"


namespace bv { namespace nrl {

// **************************
//
RenderResult::RenderResult                                      ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_renderOutputChannels( (unsigned int) RenderOutputChannelType::ROCT_TOTAL )
    , m_containsValidData( (unsigned int) RenderOutputChannelType::ROCT_TOTAL )
{
    for( unsigned int i = 0; i < m_renderOutputChannels.size(); ++i )
    {
        auto channel = new RenderOutputChannel( allocator, numTrackedRenderTargetsPerOutputType );

        m_renderOutputChannels[ i ] = channel;
        m_containsValidData[ i ] = false;
    }
}

// **************************
//
RenderResult::~RenderResult                                     ()
{
    for( auto channel : m_renderOutputChannels )
    {
        delete channel;
    }
}

// **************************
//
const RenderOutputChannel * RenderResult::GetRenderOutputChannel( RenderOutputChannelType roct ) const
{
    return m_renderOutputChannels[ ( unsigned int ) roct ];
}

// **************************
//
const RenderTarget *		RenderResult::GetActiveRenderTarget ( RenderOutputChannelType roct ) const
{
	auto channel = GetRenderOutputChannel( roct );

	return channel->GetActiveRenderTarget();
}

// **************************
//
void                    RenderResult::UpdateOutputChannels      ( const SceneVec & scenes )
{
    // FIXME: nrl - take scenes into account somehow
    { scenes; }

    for( auto channel : m_renderOutputChannels )
    {
        channel->UpdateActiveRenderTargetIdx();
    }
}

// **************************
//
bool                    RenderResult::IsActive                  ( RenderOutputChannelType roct ) const
{
    return GetRenderOutputChannel( roct )->IsActive();
}

// **************************
//
void                    RenderResult::SetIsActive               ( RenderOutputChannelType roct, bool isActive )
{
    m_renderOutputChannels[ ( unsigned int ) roct ]->SetActiveFlag( isActive );
}

// **************************
//
bool                    RenderResult::ContainsValidData         ( RenderOutputChannelType roct ) const
{
    return m_containsValidData[ (unsigned int) roct ];
}

// **************************
//
void                    RenderResult::SetContainsValidData          ( RenderOutputChannelType roct, bool containsValidData )
{
    m_containsValidData[ (unsigned int) roct ] = containsValidData;
}

} // nrl
} // bv
