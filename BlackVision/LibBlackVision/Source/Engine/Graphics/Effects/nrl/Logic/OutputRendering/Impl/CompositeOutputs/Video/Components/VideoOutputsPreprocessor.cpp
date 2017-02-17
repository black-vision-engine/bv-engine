#include "stdafx.h"

#include "VideoOutputsPreprocessor.h"

#include "Assets/Texture/TextureUtils.h"


namespace bv { namespace nrl {

// *********************************
//
VideoOutputsPreprocessor::VideoOutputsPreprocessor()
    : m_initialized( false )
{
}

// *********************************
//
const AVOutputsData &   VideoOutputsPreprocessor::Preprocess            ( NRenderContext * ctx, RenderedChannelsData * input )
{
    if( !m_initialized )
    {
        m_inputChannels.PostInitialize( input );
    
        m_initialized = true;
    }

    for( unsigned int i = 0; i < m_inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = m_inputChannels.GetVideoInputChannelAt( i );

        assert( vic->IsActive() );

        AVFramePtr frame = PrepareAVFrame( ctx, vic );

        for( auto outputID : m_inputChannels.GetAsignedVideoCardIds( vic ) )
        {
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
        // FIXME: implement
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
    auto videoFrame = channel->ReadColorTexture( ctx );

    videocards::AVFrameDescriptor desc;

	desc.width  = videoFrame->GetWidth();
	desc.height = videoFrame->GetHeight();
	desc.depth  = TextureUtils::Channels( videoFrame->GetFormat() );

	MemoryChunkPtr data = MemoryChunk::Create(1);

	desc.channels = 0;
	desc.sampleRate = 0;

	if (!m_lastFrameHasAudio)
	{
		desc.channels = audio->GetChannels();
		desc.sampleRate = audio->GetFrequency() / m_fps;

		auto audioSize = desc.sampleRate * desc.channels * audio->GetChannelDepth();

		data = MemoryChunk::Create(audioSize);

		auto ret = audio->GetBufferedData(data);
		data = std::const_pointer_cast<MemoryChunk>(ret->GetData());
	}

	m_lastFrameHasAudio = !m_lastFrameHasAudio;

	desc.fieldModeEnabled       = true;
	desc.timeCodePresent        = true;
	desc.autoGenerateTimecode   = true;

	auto frame = std::make_shared< videocards::AVFrame >( videoFrame->GetData(), data, desc );

    // FIXME: nrl - what about this? Daria, Paweuek
	frame->m_TimeCode.h = 10;
	frame->m_TimeCode.m = 22;
	frame->m_TimeCode.s = 33;
	frame->m_TimeCode.frame = 12;
  
    return frame;

}

} //bv
} //nrl
