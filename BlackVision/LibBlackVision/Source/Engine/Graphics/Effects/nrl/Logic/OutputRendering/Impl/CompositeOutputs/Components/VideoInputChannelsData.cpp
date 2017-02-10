#include "stdafx.h"

#include "VideoInputChannelsData.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Components/VideoInputChannel.h"


namespace bv { namespace nrl {

// **************************
//
VideoInputChannelsData::VideoInputChannelsData                              ( const RenderedChannelsData * renderedChannelsData )
    : m_wrappedRenderedChannelsData( renderedChannelsData )
{  
}

// **************************
//
VideoInputChannelsData::~VideoInputChannelsData                             ()
{
    for ( auto channel : m_renderChannels )
    {
        delete channel;
    }
}

// **************************
//
const VideoInputChannel *   VideoInputChannelsData::GetInputChannel         ( RenderChannelType rct ) const
{
    return m_renderChannels[ (unsigned int) rct ];
}

// **************************
//
void                        VideoInputChannelsData::InvalidateCachedTextures()
{
    for( auto channel : m_renderChannels )
    {
        channel->InvalidateCachedTexture();
    }
}

// **************************
//
void                        VideoInputChannelsData::InvalidateCachedTexture ( RenderChannelType rct )
{
    auto channel = GetInputChannel( rct );

    channel->InvalidateCachedTexture();
}

// **************************
//
Texture2DPtr                VideoInputChannelsData::ReadColorTexture        ( NRenderContext * ctx, RenderChannelType rct ) const
{
    auto channel = GetInputChannel( rct );

    return channel->ReadColorTexture( ctx );
}

// **************************
//
bool                        VideoInputChannelsData::IsActive                ( RenderChannelType rct ) const
{
    auto channel = GetInputChannel( rct );

    return channel->IsActive();
}

// **************************
//
bool                        VideoInputChannelsData::ContainsValidData       ( RenderChannelType rct ) const
{
    return m_wrappedRenderedChannelsData->ContainsValidData( rct );
}

// **************************
//
VideoInputChannelsData *    VideoInputChannelsData::Create                  ( const RenderedChannelsData * renderedChannelsData )
{
    assert( renderedChannelsData != nullptr );

    return new VideoInputChannelsData( renderedChannelsData );
}

} //nrl
} //bv
