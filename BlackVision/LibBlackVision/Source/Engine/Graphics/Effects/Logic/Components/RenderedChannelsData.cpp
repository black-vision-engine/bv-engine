#include "stdafx.h"

#include "RenderedChannelsData.h"

#include <cassert>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"


namespace bv { 

// **************************
//
RenderedChannelsData::RenderedChannelsData                                          ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType )
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
RenderedChannelsData::~RenderedChannelsData                                         ()
{
    for( auto channel : m_renderChannels )
    {
        delete channel;
    }
}

// **************************
//
const RenderChannel *       RenderedChannelsData::GetRenderChannel          ( RenderChannelType rct ) const
{
    return m_renderChannels[ ( unsigned int ) rct ];
}

// **************************
//
const RenderTarget *		RenderedChannelsData::GetActiveRenderTarget     ( RenderChannelType rct ) const
{
	auto channel = GetRenderChannel( rct );

	return channel->GetActiveRenderTarget();
}

// **************************
//
void                        RenderedChannelsData::InvalidateCachedTexture   ( RenderChannelType rct )
{
    auto channel = GetRenderChannel( rct );

    channel->InvalidateCachedTexture();
}

// **************************
//
Texture2DPtr                RenderedChannelsData::ReadColorTexture          ( Renderer * renderer, RenderChannelType rct )
{
    auto channel = GetRenderChannel( rct );

    return channel->ReadColorTexture( renderer );
}

// **************************
//
void                    RenderedChannelsData::UpdateRenderChannels          ()
{
    for( auto channel : m_renderChannels )
    {
        channel->UpdateActiveRenderTargetIdx();
    }
}

// **************************
//
bool                    RenderedChannelsData::IsActive                      ( RenderChannelType rct ) const
{
    return GetRenderChannel( rct )->IsActive();
}

// **************************
//
void                    RenderedChannelsData::SetIsActive                   ( RenderChannelType rct, bool isActive )
{
    m_renderChannels[ ( unsigned int ) rct ]->SetActiveFlag( isActive );
}

// **************************
//
bool                    RenderedChannelsData::ContainsValidData             ( RenderChannelType rct ) const
{
    return m_containsValidData[ (unsigned int) rct ];
}

// **************************
//
void                    RenderedChannelsData::SetContainsValidData          ( RenderChannelType rct, bool containsValidData )
{
    m_containsValidData[ (unsigned int) rct ] = containsValidData;
}

// **************************
//
bool                    RenderedChannelsData::ContainsValidData             ( const RenderChannel * channel ) const
{
    for( unsigned int i = 0; i < m_containsValidData.size(); ++i )
    {
        if( m_renderChannels[ i ] == channel )
        {
            return m_containsValidData[ i ];
        }
    }

    return false;
}

// **************************
//
RenderedChannelsData *  RenderedChannelsData::Create                        ( const RenderedChannelsDataDesc & desc, RenderTargetStackAllocator * allocator )
{
    RenderChannelType mapping[] = { RenderChannelType::RCT_OUTPUT_1, RenderChannelType::RCT_OUTPUT_2, RenderChannelType::RCT_OUTPUT_3, RenderChannelType::RCT_OUTPUT_4 };

    unsigned int numElts = (unsigned int) RenderChannelType::RCT_TOTAL;

    assert( (unsigned int) RenderChannelType::RCT_OUTPUT_4 == ( ( (unsigned int) RenderChannelType::RCT_TOTAL ) - 1 ) );

    auto res = new RenderedChannelsData( allocator, desc.GetNumTrackedRenderTargets() );

    for( unsigned int i = 0; i < numElts; ++i )
    {
        auto outputType = mapping[ i ];

        res->SetIsActive( outputType, desc.IsEnabled( outputType ) );
    }

    return res;
}


} // bv
