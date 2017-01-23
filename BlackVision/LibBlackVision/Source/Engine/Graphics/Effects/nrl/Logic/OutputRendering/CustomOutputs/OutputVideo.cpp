#include "stdafx.h"

#include "OutputVideo.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

#include "Engine/Audio/Resources/AudioUtils.h"
#include "Assets/Texture/TextureUtils.h"

#include "VideoCardManager.h"


namespace bv { namespace nrl {

// **************************
//
OutputVideo::OutputVideo     ( unsigned int width, unsigned int height )
    : OutputInstance( width, height )
    , m_audioData( nullptr )
	, m_lastFrameHasAudio( true )
    , m_videoRT( nullptr )
    , m_videoTexture( nullptr )
{
    // FIXME: nrl - should be read from video cards configuration
    m_fps = 25; 

    auto samplesPerFrame    = audio::AudioUtils::DEFAULT_SAMPLE_RATE / m_fps;
    auto bytesPerSample     = audio::AudioUtils::ChannelDepth( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT );
    auto numChannels        = audio::AudioUtils::ChannelsCount( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT );

    auto memChunkSize       = numChannels * samplesPerFrame * bytesPerSample;
                        
    m_audioData = MemoryChunk::Create( memChunkSize );
}

// **************************
//
OutputVideo::~OutputVideo    ()
{
    delete m_videoRT;
}

// **************************
//
void    OutputVideo::ProcessFrameData( NRenderContext * ctx, RenderResult * input )
{
    auto rct = GetActiveRenderChannel();

	// FIXME: nrl - this can be solved some other way around - Pawelek has to decide (e.g. when the required render channel is not active black rame is displayed or process frame data does nothing)
    assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );

    auto outputRT = input->GetActiveRenderTarget( rct );

    // FIXME: nrl - deferred initialization, a bit too generic right now
    if( outputRT->Width() != GetWidth() || outputRT->Height() != GetHeight() )
    {
        if ( m_videoRT == nullptr )
        {
            m_videoRT = allocator( ctx )->CreateCustomRenderTarget( outputRT->Width(), outputRT->Height(), RenderTarget::RTSemantic::S_DRAW_READ );
        }

        assert( false ); // FIXME: nrl - implement
        assert( outputRT->Width() == m_videoRT->Width() && outputRT->Height() == m_videoRT->Height() );
    }

	auto videoFrame = input->ReadColorTexture( renderer( ctx ), rct );

	auto avFrame = PrepareAVFrame( audio( ctx ), videoFrame );

	{
		// FIXME: nrl - ask Witek about this one
		//HPROFILER_SECTION("QueueFrame", PROFILER_THREAD1);
		videocards::VideoCardManager::Instance().QueueFrame( avFrame );
	}
}

// **************************
//
videocards::AVFramePtr  OutputVideo::PrepareAVFrame ( audio::AudioRenderer * audio, Texture2DPtr videoFrame )
{
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

} //nrl
} //bv
