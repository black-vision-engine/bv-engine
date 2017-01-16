#include "stdafx.h"

#include "RenderResult.h"

#include <cassert>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderChannel.h"


namespace bv { namespace nrl {

// **************************
//
RenderResult::RenderResult                                      ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_renderChannels( (unsigned int) RenderChannelType::RCT_TOTAL )
    , m_containsValidData( (unsigned int) RenderChannelType::RCT_TOTAL )
{
    for( unsigned int i = 0; i < m_renderChannels.size(); ++i )
    {
        auto channel = new RenderChannel( allocator, numTrackedRenderTargetsPerOutputType );

        m_renderChannels[ i ] = channel;
        m_containsValidData[ i ] = false;
    }
}

// **************************
//
RenderResult::~RenderResult                                     ()
{
    for( auto channel : m_renderChannels )
    {
        delete channel;
    }
}

// **************************
//
const RenderChannel *       RenderResult::GetRenderChannel      ( RenderChannelType rct ) const
{
    return m_renderChannels[ ( unsigned int ) rct ];
}

// **************************
//
const RenderTarget *		RenderResult::GetActiveRenderTarget ( RenderChannelType rct ) const
{
	auto channel = GetRenderChannel( rct );

	return channel->GetActiveRenderTarget();
}

// **************************
//
void                    RenderResult::UpdateRenderChannels      ()
{
    for( auto channel : m_renderChannels )
    {
        channel->UpdateActiveRenderTargetIdx();
    }
}

// **************************
//
bool                    RenderResult::IsActive                  ( RenderChannelType rct ) const
{
    return GetRenderChannel( rct )->IsActive();
}

// **************************
//
void                    RenderResult::SetIsActive               ( RenderChannelType rct, bool isActive )
{
    m_renderChannels[ ( unsigned int ) rct ]->SetActiveFlag( isActive );
}

// **************************
//
bool                    RenderResult::ContainsValidData         ( RenderChannelType rct ) const
{
    return m_containsValidData[ (unsigned int) rct ];
}

// **************************
//
void                    RenderResult::SetContainsValidData      ( RenderChannelType rct, bool containsValidData )
{
    m_containsValidData[ (unsigned int) rct ] = containsValidData;
}

} // nrl
} // bv
