#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"

#include "Engine/Audio/Resources/AudioUtils.h"
#include "Assets/Texture/TextureUtils.h"

#include "Services/BVServiceProvider.h"

#include "VideoCardManager.h"


namespace bv { namespace nrl {

// *********************************
//
VideoOutputsPreprocessor::VideoOutputsPreprocessor()
    : m_initialized( false )
    , m_lcmFPS( 0 )
    , m_currentAVFrame( nullptr )
{}

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess            ( NRenderContext * ctx, RenderedChannelsData * input )
{
    if( !m_initialized )
    {
        m_inputChannels.PostInitialize( input );
    
        m_initialized = true;

        m_lcmFPS = BVServiceProvider::GetInstance().GetVideoCardManager()->GetRequiredFPS();
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
AVFramePtr              VideoOutputsPreprocessor::PrepareAVFrame        ( NRenderContext * ctx, const VideoInputChannel * channel )
{
    auto aud = audio( ctx );

    auto videoFrame = channel->ReadColorTexture( ctx );
    
    videocards::AVFrameDescriptor desc;

	desc.width  = videoFrame->GetWidth();
	desc.height = videoFrame->GetHeight();
	desc.depth  = TextureUtils::Channels( videoFrame->GetFormat() );

	desc.channels = 0;
	desc.sampleRate = 0;

    //if ( !channel->LastFrameHadAudio() )
	desc.channels = aud->GetChannels();
	desc.sampleRate = aud->GetFrequency() / m_lcmFPS;

    if( !m_currentAVFrame )
    {
        m_currentAVFrame = videocards::AVFrame::Create();

        // FIXME: nrl - what about this? Daria, Paweuek
        m_currentAVFrame->m_TimeCode.h = 10;
        m_currentAVFrame->m_TimeCode.m = 22;
        m_currentAVFrame->m_TimeCode.s = 33;
        m_currentAVFrame->m_TimeCode.frame = 12;
    }

    m_currentAVFrame->m_videoData = videoFrame->GetData();
    m_currentAVFrame->m_desc = desc;

    auto audioSize = desc.sampleRate * desc.channels * aud->GetChannelDepth();

    if( !m_currentAVFrame->m_audioData || m_currentAVFrame->m_audioData->Size() != audioSize )
        m_currentAVFrame->m_audioData = MemoryChunk::Create( audioSize );

	auto ret = aud->GetBufferedData( std::const_pointer_cast< MemoryChunk >( m_currentAVFrame->m_audioData ),
                                     channel->GetWrappedChannel()->AccessRenderChannelAudioEntities() );

    //channel->ToggleLastFrameHadAudio();

	desc.fieldModeEnabled       = true;
	desc.timeCodePresent        = true;
	desc.autoGenerateTimecode   = true;

    return m_currentAVFrame;
}

} //bv
} //nrl
