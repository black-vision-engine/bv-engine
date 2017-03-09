#include "stdafx.h"

#include "MockVideoHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NOutputState.h"

#include "Engine/Audio/Resources/AudioUtils.h"
#include "Assets/Texture/TextureUtils.h"

#include "VideoCardManager.h"


namespace bv { namespace nrl {

// **************************
//
MockVideoHandler::MockVideoHandler                          ( unsigned int width, unsigned int height )
    : m_width( width )
    , m_height( height )
    , m_activeRenderOutput( 1 )
    , m_audioData( nullptr )
	, m_lastFrameHasAudio( true )
    , m_mixChannelsEffect( nullptr )
    , m_videoRT( nullptr )
    , m_videoTexture( nullptr )
{
    // FIXME: nrl - should be read from video cards configuration
    m_fps = 25; 

    auto samplesPerFrame    = audio::AudioUtils::DEFAULT_SAMPLE_RATE / m_fps;
    auto bytesPerSample     = audio::AudioUtils::ChannelDepth( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT );
    auto numChannels        = audio::AudioUtils::ChannelsCount( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT );

    auto memChunkSize       = numChannels * samplesPerFrame * bytesPerSample;
                        
    m_mixChannelsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_MIX_CHANNELS );

    m_audioData = MemoryChunk::Create( memChunkSize );
}

// **************************
//
MockVideoHandler::~MockVideoHandler                         ()
{
    delete m_videoRT;
    delete m_mixChannelsEffect;
}

// **************************
//
void    MockVideoHandler::HandleFrameData                   ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * inputChannel )
{
    //1. Prepare memory representation of current frame
    auto videoFrame = PrepareFrame( state, ctx, inputChannel );

    //2. Process memory representation of current frame
    ProcessFrame( ctx, videoFrame );
}

// **************************
//
NFullscreenEffectComponentStatePtr  MockVideoHandler::GetInternalFSEState       ()
{
    return m_mixChannelsEffect->GetState();
}

// *********************************
//
Texture2DPtr    MockVideoHandler::PrepareFrame              ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * inputChannel )
{
    auto inputRenderTarget = inputChannel->GetActiveRenderTarget();  

    Texture2DPtr outputFrame;

    bool wb = m_width == inputRenderTarget->Width();
    bool hb = m_height == inputRenderTarget->Height();
    bool sb = state.RepresentsDefaultTexture();

    if( wb && hb && sb )
    {
        outputFrame = ReadDefaultTexture( ctx, inputChannel );
    }
    else
    {
        outputFrame = ReadMixChannelsTexture( ctx, inputRenderTarget );
    }

    return outputFrame;
}

// *********************************
//
void            MockVideoHandler::ProcessFrame               ( NRenderContext * ctx, Texture2DPtr frame )
{
	auto avFrame = PrepareAVFrame( audio( ctx ), frame );

	{
		// FIXME: nrl - ask Witek about this one
		//HPROFILER_SECTION("QueueFrame", PROFILER_THREAD1);
		//videocards::VideoCardManager::Instance().QueueFrame( avFrame );
	}
}

// *********************************
//
Texture2DPtr    MockVideoHandler::ReadDefaultTexture         ( NRenderContext * ctx, const RenderChannel * inputChannel )
{
	return inputChannel->ReadColorTexture( renderer( ctx ) );
}

// *********************************
// FIXME: implement is as a part of some readback cache
Texture2DPtr    MockVideoHandler::ReadMixChannelsTexture     ( NRenderContext * ctx, const RenderTarget * inputRenderTarget )
{
    if( m_videoRT == nullptr )
    {
        m_videoRT = allocator( ctx )->CreateCustomRenderTarget( m_width, m_height, RenderTarget::RTSemantic::S_DRAW_READ );
    }

    m_activeRenderOutput.SetEntry( 0, inputRenderTarget );
    m_mixChannelsEffect->Render( ctx, m_videoRT, m_activeRenderOutput );

    renderer( ctx )->ReadColorTexture( 0, m_videoRT, m_videoTexture );

    return m_videoTexture;
}

// **************************
//
AVFramePtr  MockVideoHandler::PrepareAVFrame                ( audio::AudioRenderer * audio, Texture2DPtr videoFrame )
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

		data = MemoryChunk::Create( audioSize );

		auto ret = audio->GetBufferedData( data );
		data = std::const_pointer_cast< MemoryChunk >( ret->GetData() );
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
