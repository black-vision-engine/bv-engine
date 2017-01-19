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
void    OutputVideo::ProcessFrameData(NRenderContext * ctx, RenderResult * result)
{
	// FIXME: nrl - default logic uses only RenderChannelType::RCT_OUTPUT_1 result channel to show the results
	// FIXME: nrl - implement more logic here
	auto rct = RenderChannelType::RCT_OUTPUT_1;
	assert(result->IsActive(rct) && result->ContainsValidData(rct));

	auto videoFrame = result->ReadColorTexture(renderer(ctx), rct);
	auto avFrame = PrepareAVFrame(audio(ctx), videoFrame);

	{
		// FIXME: nrl - ask Witek about this one
		//HPROFILER_SECTION("QueueFrame", PROFILER_THREAD1);
		videocards::VideoCardManager::Instance().QueueFrame(avFrame);
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
