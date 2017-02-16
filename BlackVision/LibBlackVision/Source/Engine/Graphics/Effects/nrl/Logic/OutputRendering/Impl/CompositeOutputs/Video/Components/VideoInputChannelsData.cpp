#include "stdafx.h"

#include "VideoInputChannelsData.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"


namespace bv { namespace nrl {

// **************************
//
VideoInputChannelsData::VideoInputChannelsData                              ()
{  
//    for ( unsigned int i = (unsigned int) RenderChannelType::RCT_OUTPUT_1; i < (unsigned int) RenderChannelType::RCT_TOTAL; ++i )
//    {
//        auto c = renderedChannelsData->GetRenderChannel( (RenderChannelType) i );
//
//        { c; }
//        // FIXME: nrl - implement via passing w, h of corresponding outputs
////        auto vic = VideoInputChannel::Create( 
//    }
}

// **************************
//
VideoInputChannelsData::~VideoInputChannelsData                             ()
{
    //for ( auto channel : m_renderChannels )
    //{
    //    delete channel;
    //}
}

// **************************
//
void    VideoInputChannelsData::PreInitialize                               ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    // FIXME: deferred initialization to be implemented
    assert( m_outputToChannelsMapping.size() == 0 );
    assert( m_videoInputChannels.size() == 0 );

    // VInputChannel
    for( auto & uo : uniqueOutputSetups )
    {
        { uo; }
        m_videoInputChannels.push_back( nullptr );
    }

    for( auto & m : mapping )
    {
        m_outputToChannelsMapping[ m.first ] = nullptr;
    }

}

//// **************************
////
//const VideoInputChannel *   VideoInputChannelsData::GetInputChannel         ( RenderChannelType rct ) const
//{
//    return m_renderChannels[ (unsigned int) rct ];
//}
//
//// **************************
////
//void                        VideoInputChannelsData::InvalidateCachedTextures()
//{
//    for( auto channel : m_renderChannels )
//    {
//        channel->InvalidateCachedTexture();
//    }
//}
//
//// **************************
////
//void                        VideoInputChannelsData::InvalidateCachedTexture ( RenderChannelType rct )
//{
//    auto channel = GetInputChannel( rct );
//
//    channel->InvalidateCachedTexture();
//}
//
//// **************************
////
//Texture2DPtr                VideoInputChannelsData::ReadColorTexture        ( NRenderContext * ctx, RenderChannelType rct ) const
//{
//    auto channel = GetInputChannel( rct );
//
//    return channel->ReadColorTexture( ctx );
//}
//
//// **************************
////
//bool                        VideoInputChannelsData::IsActive                ( RenderChannelType rct ) const
//{
//    auto channel = GetInputChannel( rct );
//
//    return channel->IsActive();
//}
//
//// **************************
////
//bool                        VideoInputChannelsData::ContainsValidData       ( RenderChannelType rct ) const
//{
//    return m_wrappedRenderedChannelsData->ContainsValidData( rct );
//}
//
//// **************************
////
//VideoInputChannelsData *    VideoInputChannelsData::Create                  ( const std::vector< OutputStaticData > & uniqueOutputSetups, const std::hash_map< unsigned int, unsigned int > & mapping )
//{
//    assert( renderedChannelsData != nullptr );
//
//    return new VideoInputChannelsData( renderedChannelsData );
//}

} //nrl
} //bv
