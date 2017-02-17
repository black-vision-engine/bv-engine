#include "stdafx.h"

#include <set>

#include "VideoInputChannelsData.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"


namespace bv { namespace nrl {

// **************************
//
VideoInputChannelsData::VideoInputChannelsData                              ()
    : m_preInitialized( false )
    , m_postInitialized( false )
    , m_originalRenderedChannelsData( nullptr )
{  
}

// **************************
//
VideoInputChannelsData::~VideoInputChannelsData                             ()
{
    for ( auto channel : m_videoInputChannels )
    {
        delete channel;
    }
}

// **************************
//
void    VideoInputChannelsData::PreInitialize                               ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    // FIXME: deferred initialization to be implemented
    assert( m_preInitialized == false );
    assert( m_postInitialized == false );

    m_preUniqueOutputSetups = uniqueOutputSetups;
    m_preMapping = mapping;

    m_preInitialized = true;
}

// **************************
//
void    VideoInputChannelsData::PostInitialize                              ( const RenderedChannelsData * rcd )
{
    assert( m_preInitialized == true );
    assert( m_postInitialized == false );
    assert( m_videoInputChannels.size() == 0 );
    assert( m_outputToChannelsMapping.size() == 0 );
    assert( m_channelToOutputMaping.size() == 0 );
    assert( m_originalRenderedChannelsData == nullptr );

    m_originalRenderedChannelsData = rcd;

    // Initialize unique video output setups
    for( auto & s : m_preUniqueOutputSetups )
    {
        auto vrc = VideoInputChannel::Create( rcd->GetRenderChannel( s.selectedRenderedChannel ), s.width, s.height );

        m_videoInputChannels.push_back( vrc );
    }

    // Intialize mapping between video card outputs and unique setups
    for( auto & e : m_preMapping )
    {
        auto videoCardID    = e.first;
        auto outputIdx      = e.second;

        auto vrc            = m_videoInputChannels[ outputIdx ];

        m_outputToChannelsMapping[ videoCardID ] = vrc;
    }

    // Add inverted mapping from channels to corresponding video outputs (multiple outputs per channel)
    for ( auto vce : m_outputToChannelsMapping )
    {
        auto channel    = vce.second;
        auto outputId   = vce.first;

        m_channelToOutputMaping[ channel ].push_back( outputId );
    }

    // Remove duplicates
    unsigned int sum = 0;

    for( auto & e : m_channelToOutputMaping )
    {
        std::set< VideoCardID > s( e.second.begin(), e.second.end() );
        e.second = VideoCardIDVec( s.begin(), s.end() );
    
        sum += (unsigned int) e.second.size();
    }

    assert( sum == (unsigned int) m_outputToChannelsMapping.size() );

    m_postInitialized = false;
}

// **************************
//
const VideoInputChannel *   VideoInputChannelsData::GetInputChannel         ( unsigned int videoOutputID ) const
{
    assert( m_outputToChannelsMapping.find( videoOutputID ) != m_outputToChannelsMapping.end() );

    auto it = m_outputToChannelsMapping.find( videoOutputID );

    return it->second;
}

// **************************
//
VideoInputChannel *         VideoInputChannelsData::AccessInputChannel      ( unsigned int videoOutputID )
{
    assert( m_outputToChannelsMapping.find( videoOutputID ) != m_outputToChannelsMapping.end() );

    auto it = m_outputToChannelsMapping.find( videoOutputID );

    return it->second;
}

// **************************
//
unsigned int                VideoInputChannelsData::GetNumVideoInputChannels() const
{
    return (unsigned int) m_videoInputChannels.size();
}

// **************************
//
const VideoInputChannel *   VideoInputChannelsData::GetVideoInputChannelAt  ( unsigned int idx ) const
{
    assert( idx < GetNumVideoInputChannels() );

    return m_videoInputChannels[ idx ];
}

// **************************
//
const VideoCardIDVec &      VideoInputChannelsData::GetAsignedVideoCardIds  ( const VideoInputChannel * vic ) const
{
    assert( m_channelToOutputMaping.find( vic ) != m_channelToOutputMaping.end() );

    auto it = m_channelToOutputMaping.find( vic );

    return it->second;
}

// **************************
//
bool                        VideoInputChannelsData::LastFrameHadAudio       ( unsigned int videoOutputID ) const
{
    auto channel = GetInputChannel( videoOutputID );

    return channel->LastFrameHadAudio();
}

// **************************
//
void                        VideoInputChannelsData::ToggleLastFrameHadAudio ( unsigned int videoOutputID ) const
{
    auto channel = GetInputChannel( videoOutputID );

    channel->ToggleLastFrameHadAudio();
}

// **************************
//
void                        VideoInputChannelsData::InvalidateCachedTextures()
{
    for( auto channel : m_videoInputChannels )
    {
        channel->InvalidateCachedTexture();
    }
}

// **************************
//
void                        VideoInputChannelsData::InvalidateCachedTexture ( unsigned int videoOutputID )
{
    auto channel = GetInputChannel( videoOutputID );

    channel->InvalidateCachedTexture();
}

// **************************
//
Texture2DPtr                VideoInputChannelsData::ReadColorTexture        ( NRenderContext * ctx, unsigned int videoOutputID ) const
{
    auto channel = GetInputChannel( videoOutputID );

    return channel->ReadColorTexture( ctx );
}

// **************************
//
bool                        VideoInputChannelsData::IsActive                ( unsigned int videoOutputID ) const
{
    auto channel = GetInputChannel( videoOutputID );

    return channel->IsActive();
}

// **************************
//
bool                        VideoInputChannelsData::ContainsValidData       ( unsigned int videoOutputID ) const
{
    auto vc = m_outputToChannelsMapping.find( videoOutputID )->second;

    return m_originalRenderedChannelsData->ContainsValidData( vc->GetWrappedChannel() );
}

} //nrl
} //bv
