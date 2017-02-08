#include "stdafx.h"

#include "OutputVideo.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

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
OutputVideo::~OutputVideo    ()
{
    delete m_videoRT;
    delete m_mixChannelsEffect;
}

// **************************
//
void    OutputVideo::ProcessFrameData( NRenderContext * ctx, RenderResult * input )
{
    //1. Update internal output state
    // FIXME: nrl - this is a bit of an overkill, but let's update it every frame here
    UpdateEffectValues();

    //2. Prepare memory representation of current frame
    auto videoFrame = PrepareFrame( ctx, input );

    //3. Process memory representation of current frame
    ProcessFrame( ctx, videoFrame );
}

// *********************************
//
void            OutputVideo::UpdateEffectValues         ()
{
    auto state = m_mixChannelsEffect->GetState();
    
    auto mappingVal = state->GetValueAt( 0 ); assert( mappingVal->GetName() == "channelMapping" );
    auto maskVal    = state->GetValueAt( 1 ); assert( maskVal->GetName() == "channelMask" );

    auto mapping = GetChannelMapping();
    auto mask = GetChannelMask();

    QueryTypedValue< ValueIntPtr >( mappingVal )->SetValue( mapping );
    QueryTypedValue< ValueVec4Ptr >( maskVal )->SetValue( mask );
}

// *********************************
//
Texture2DPtr    OutputVideo::PrepareFrame               ( NRenderContext * ctx, RenderResult * input )
{
    auto rct = GetActiveRenderChannel();
    assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );

    auto inputRenderTarget = input->GetActiveRenderTarget( rct );  

    Texture2DPtr outputFrame;

    if( GetWidth() == inputRenderTarget->Width() && GetHeight() == inputRenderTarget->Height() && AccessOutputState().RepresentsDefaultTexture() )
    {
        outputFrame = ReadDefaultTexture( ctx, input, rct );
    }
    else
    {
        outputFrame = ReadMixChannelsTexture( ctx, inputRenderTarget );
    }

    return outputFrame;
}

// *********************************
//
void            OutputVideo::ProcessFrame               ( NRenderContext * ctx, Texture2DPtr frame )
{
	auto avFrame = PrepareAVFrame( audio( ctx ), frame );

	{
		// FIXME: nrl - ask Witek about this one
		//HPROFILER_SECTION("QueueFrame", PROFILER_THREAD1);
		videocards::VideoCardManager::Instance().QueueFrame( avFrame );
	}
}

// *********************************
//
Texture2DPtr    OutputVideo::ReadDefaultTexture         ( NRenderContext * ctx, RenderResult * input, RenderChannelType rct )
{
	return input->ReadColorTexture( renderer( ctx ), rct );
}

// *********************************
//
Texture2DPtr    OutputVideo::ReadMixChannelsTexture     ( NRenderContext * ctx, const RenderTarget * inputRenderTarget )
{
    if( m_videoRT == nullptr )
    {
        auto w = GetWidth();
        auto h = GetHeight();

        m_videoRT = allocator( ctx )->CreateCustomRenderTarget( w, h, RenderTarget::RTSemantic::S_DRAW_READ );
    }

    m_activeRenderOutput.SetEntry( 0, inputRenderTarget );
    m_mixChannelsEffect->Render( ctx, m_videoRT, m_activeRenderOutput );

    renderer( ctx )->ReadColorTexture( 0, m_videoRT, m_videoTexture );

    return m_videoTexture;
}

// **************************
//
AVFramePtr  OutputVideo::PrepareAVFrame ( audio::AudioRenderer * audio, Texture2DPtr videoFrame )
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
