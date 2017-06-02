#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"

#include "Engine/Audio/Resources/AudioUtils.h"
#include "Engine/Audio/Resources/AudioMixer.h"
#include "Assets/Texture/TextureUtils.h"

#include "Services/BVServiceProvider.h"

#include "VideoCardManager.h"


namespace bv { 

namespace
{
const SizeType      BUFFER_SIZE = 10;
}

// *********************************
//
VideoOutputsPreprocessor::VideoOutputsPreprocessor()
    : m_initialized( false )
    , m_lcmFPS( 0 )
    , m_audioMixer( nullptr )
{}

// *********************************
//
VideoOutputsPreprocessor::~VideoOutputsPreprocessor   ()
{
    delete m_audioMixer;
}

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess            ( RenderContext * ctx, RenderedChannelsData * input )
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
AVFramePtr              VideoOutputsPreprocessor::PrepareAVFrame        ( RenderContext * ctx, const VideoInputChannel * vic )
{
    if( m_currentAVFrames.find( vic ) != m_currentAVFrames.end() )
    {
        m_avFramesBuffer[ vic ].push_back( m_currentAVFrames[ vic ] );
    }

    auto avFrame = m_avFramesBuffer[ vic ].front();
    m_currentAVFrames[ vic ] = avFrame;

    auto videoFrame = vic->ReadColorTexture( ctx );
    avFrame->m_videoData = videoFrame->GetData();

    if( !m_audioMixer )
        m_audioMixer = new audio::AudioMixer( vic->GetWrappedChannel()->GetAudioRenderChannelData() );

    m_audioMixer->MixAudioBuffersVecs( std::const_pointer_cast< MemoryChunk >( avFrame->m_audioData ) );

    return avFrame;
}

// *********************************
//
void                  VideoOutputsPreprocessor::InitializeAVBuffers   ( RenderContext * ctx )
{
    auto audio = audio_renderer( ctx );

    auto audioFrameSize = audio->GetChannels() * audio->GetChannelDepth() * audio->GetFrequency() / m_lcmFPS;

    for( unsigned int i = 0; i < m_inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = m_inputChannels.GetVideoInputChannelAt( i );
        assert( vic->IsActive() );

        AVFrameDescriptor desc;

        desc.width = vic->GetWidth();
        desc.height = vic->GetHeight();
        desc.depth = TextureUtils::Channels( vic->GetFormat() );
        desc.channels = audio->GetChannels();
        desc.sampleRate = audio->GetFrequency() / m_lcmFPS;

        // FIXME: values are hardcoded.  // FIXME: https://www.pivotaltracker.com/story/show/145508031
        //desc.fieldModeEnabled = true;
        //desc.timeCodePresent = true;
        //desc.autoGenerateTimecode = true;

        m_avFramesBuffer[ vic ] = boost::circular_buffer< AVFramePtr >( BUFFER_SIZE );

        for( SizeType i = 0; i < BUFFER_SIZE; ++i )
        {
            auto avFrame = AVFrame::Create();
            avFrame->m_audioData = MemoryChunk::Create( audioFrameSize );
            avFrame->m_desc = desc;

            // FIXME: values are hardcoded.  // FIXME: https://www.pivotaltracker.com/story/show/145508031
            //avFrame->m_TimeCode.h = 10;
            //avFrame->m_TimeCode.m = 22;
            //avFrame->m_TimeCode.s = 33;
            //avFrame->m_TimeCode.frame = 12;

            m_avFramesBuffer[ vic ].push_back( avFrame );
        }
    }

}

} //bv
