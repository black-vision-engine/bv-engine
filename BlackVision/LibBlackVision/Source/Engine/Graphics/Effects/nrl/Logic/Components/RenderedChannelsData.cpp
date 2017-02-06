#include "stdafx.h"

#include "RenderedChannelsData.h"

#include <cassert>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"


namespace bv { namespace nrl {

// **************************
//
RenderedChannelsData::RenderedChannelsData                                          ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutputType )
    : m_renderChannels( (unsigned int) RenderChannelType::RCT_TOTAL )
    , m_containsValidData( (unsigned int) RenderChannelType::RCT_TOTAL )
    , m_cachedReadbackTextures( (unsigned int) RenderChannelType::RCT_TOTAL )
    , m_cachedReadbackUpToDate( (unsigned int) RenderChannelType::RCT_TOTAL )
{
    for( unsigned int i = 0; i < m_renderChannels.size(); ++i )
    {
        auto channel = new RenderChannel( allocator, numTrackedRenderTargetsPerOutputType );

        m_renderChannels[ i ] = channel;
        m_containsValidData[ i ] = false;

        m_cachedReadbackTextures[ i ] = nullptr;
        m_cachedReadbackUpToDate[ i ] = false;
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
    m_cachedReadbackUpToDate[ ( unsigned int) rct ] = false;
}

// **************************
//
Texture2DPtr                RenderedChannelsData::ReadColorTexture          ( Renderer * renderer, RenderChannelType rct )
{
    unsigned int idx = (unsigned int) rct;

    auto & tex = m_cachedReadbackTextures[ idx ];

    if( !m_cachedReadbackUpToDate[ idx ] )
    {
        auto rt = GetActiveRenderTarget( rct );

        {
            // FIXME: nrl - ask Witek about this one
	        //HPROFILER_SECTION( "ReadColorTexture", PROFILER_THREAD1 );
            renderer->ReadColorTexture( 0, rt, tex );
        }

        m_cachedReadbackUpToDate[ idx ] = true;
    }

    return tex;
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
// FIXME: implement
RenderedChannelsData *  RenderedChannelsData::Create                        ( const RenderedChannelsDataDesc & desc, RenderTargetStackAllocator * allocator )
{
    // return new RenderedChannelsData();
    { desc; }
    { allocator; }

    return nullptr;
}

} // nrl
} // bv
