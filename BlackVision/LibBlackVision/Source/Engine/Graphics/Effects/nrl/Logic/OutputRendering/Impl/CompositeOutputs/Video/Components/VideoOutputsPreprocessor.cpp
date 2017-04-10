#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"

#include "Engine/Audio/Resources/AudioUtils.h"
#include "Assets/Texture/TextureUtils.h"

#include "Services/BVServiceProvider.h"

#include "VideoCardManager.h"


namespace bv { namespace nrl {

namespace
{
const SizeType      BUFFER_SIZE = 10;
}

// *********************************
//
VideoOutputsPreprocessor::VideoOutputsPreprocessor()
    : m_initialized( false )
    , m_lcmFPS( 0 )
{}

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess            ( NRenderContext * ctx, RenderedChannelsData * input )
{
    if( !m_initialized )
    {
        m_inputChannels.PostInitialize( input );
    
        m_lcmFPS = BVServiceProvider::GetInstance().GetVideoCardManager()->GetRequiredFPS();

        InitializeAVBuffers( ctx );

        m_initialized = true;
    }

    for( unsigned int i = 0; i < m_inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = m_inputChannels.GetVideoInputChannelAt( i );

        assert( vic->IsActive() );

        AVFramePtr frame = PrepareAVFrame( ctx, vic );

        for( auto outputID : m_inputChannels.GetAsignedVideoCardIds( vic ) )
        {
            assert( m_inputChannels.ContainsValidData( outputID ) );

            m_avFrames.SetAVFrame( outputID, frame );
        }
    }

    return m_avFrames;
}

// *********************************
//
void                    VideoOutputsPreprocessor::InvalidateCachedData  ()
{
    if( m_initialized )
    {
        m_inputChannels.InvalidateCachedTextures();    
    }
}

// *********************************
//
void                    VideoOutputsPreprocessor::Initialize            ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    assert( m_avFrames.GetNumEntries() == 0 );    

    for( auto & a : mapping )
    {
        m_avFrames.SetAVFrame( a.first, nullptr );
    }

    m_inputChannels.PreInitialize( uniqueOutputSetups, mapping );
}

// *********************************
//
AVFramePtr              VideoOutputsPreprocessor::PrepareAVFrame        ( NRenderContext * ctx, const VideoInputChannel * vic )
{
    if( m_currentAVFrames.find( vic ) != m_currentAVFrames.end() )
        m_avFramesBuffer[ vic ].push_back( m_currentAVFrames[ vic ] );

    auto avFrame = m_avFramesBuffer[ vic ].front();
    m_currentAVFrames[ vic ] = avFrame;

    auto videoFrame = vic->ReadColorTexture( ctx );
    avFrame->m_videoData = videoFrame->GetData();

    auto aud = audio( ctx );
	auto ret = aud->GetBufferedData( std::const_pointer_cast< MemoryChunk >( avFrame->m_audioData ),
                                     vic->GetWrappedChannel()->AccessRenderChannelAudioEntities() );

    return avFrame;
}

// *********************************
//
void                  VideoOutputsPreprocessor::InitializeAVBuffers   ( NRenderContext * ctx )
{
    auto aud = audio( ctx );

    auto audioFrameSize = aud->GetChannels() * aud->GetChannelDepth() * aud->GetFrequency() / m_lcmFPS;

    for( unsigned int i = 0; i < m_inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = m_inputChannels.GetVideoInputChannelAt( i );
        assert( vic->IsActive() );

        videocards::AVFrameDescriptor desc;

        desc.width = vic->GetWidth();
        desc.height = vic->GetHeight();
        desc.depth = TextureUtils::Channels( vic->GetFormat() );
        desc.channels = aud->GetChannels();
        desc.sampleRate = aud->GetFrequency() / m_lcmFPS;

        // FIXME: values are hardcoded.
        desc.fieldModeEnabled = true;
        desc.timeCodePresent = true;
        desc.autoGenerateTimecode = true;

        m_avFramesBuffer[ vic ] = boost::circular_buffer< AVFramePtr >( BUFFER_SIZE );

        for( SizeType i = 0; i < BUFFER_SIZE; ++i )
        {
            auto avFrame = videocards::AVFrame::Create();
            avFrame->m_audioData = MemoryChunk::Create( audioFrameSize );
            avFrame->m_desc = desc;

            // FIXME: values are hardcoded.
            avFrame->m_TimeCode.h = 10;
            avFrame->m_TimeCode.m = 22;
            avFrame->m_TimeCode.s = 33;
            avFrame->m_TimeCode.frame = 12;

            m_avFramesBuffer[ vic ].push_back( avFrame );
        }
    }

}

} //bv
} //nrl

