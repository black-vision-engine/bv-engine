#include "stdafx.h"

#include "VideoInputChannel.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoResizedInputChannel.h"

#include "Util/Audio/AudioUtils.h"
#include "Assets/Texture/TextureUtils.h"


namespace bv {

// **************************
//
VideoInputChannel::VideoInputChannel                            ( const RenderChannel * wrappedChannel )
    : m_wrappedRenderChannel( wrappedChannel )
    , m_lastFrameHadAudio( true )
{
}

// **************************
//
VideoInputChannel::~VideoInputChannel                           ()
{
}

// **************************
//
bool            VideoInputChannel::IsActive                     () const
{
    return m_wrappedRenderChannel->IsActive();
}

// **************************
//
bool            VideoInputChannel::LastFrameHadAudio            () const
{
    return m_lastFrameHadAudio;
}

// **************************
//
void            VideoInputChannel::ToggleLastFrameHadAudio      () const
{
    m_lastFrameHadAudio = !m_lastFrameHadAudio;
}

// **************************
//
Texture2DPtr    VideoInputChannel::ReadColorTexture             ( RenderContext * ctx ) const
{
    return m_wrappedRenderChannel->ReadColorTexture( renderer( ctx ) );
}

// **************************
//
void            VideoInputChannel::InvalidateCachedTexture      () const
{
    // Intentionally left empty as all textures cached in wrapped channels are up-to-date (and have already been read e.g. for preview or shm)
}

// **************************
//
const RenderChannel *   VideoInputChannel::GetWrappedChannel    () const
{
    return m_wrappedRenderChannel;
}

// **************************
//
VideoInputChannel *  VideoInputChannel::Create                  ( const RenderChannel * wrappedChannel, unsigned int width, unsigned int height )
{
    auto rt = wrappedChannel->GetActiveRenderTarget();

    auto inputWidth = rt->Width();
    auto inputHeight = rt->Height();

    VideoInputChannel * res = nullptr;

    if( width == inputWidth && height == inputHeight )
    {
        res = new VideoInputChannel( wrappedChannel );
    }
    else
    {
        res = new VideoResizedInputChannel( wrappedChannel, width, height );
    }

    // FIXME: nrl - should be read from video cards configuration or remove or do something making sense, @see: VideoOutputsPreprocessor::PrepareAVFrame
    //static unsigned int FPS_HACK = 25;

    //auto samplesPerFrame    = audio::AudioUtils::DEFAULT_SAMPLE_RATE / FPS_HACK;
    //auto bytesPerSample     = audio::AudioUtils::ChannelDepth( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT );
    //auto numChannels        = audio::AudioUtils::ChannelsCount( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT );

    //auto memChunkSize       = numChannels * samplesPerFrame * bytesPerSample;

    //res->m_audioData        = MemoryChunk::Create( memChunkSize );

    return res;
}

// **************************
//
UInt32              VideoInputChannel::GetWidth                    () const
{
    return m_wrappedRenderChannel->GetActiveRenderTarget()->Width();
}

// **************************
//
UInt32              VideoInputChannel::GetHeight                   () const
{
    return m_wrappedRenderChannel->GetActiveRenderTarget()->Height();
}

// **************************
//
TextureFormat       VideoInputChannel::GetFormat                   () const
{
    return m_wrappedRenderChannel->GetActiveRenderTarget()->Format();
}

// **************************
//
SizeType             VideoInputChannel::GetRawFrameSize             () const
{
    return m_wrappedRenderChannel->GetActiveRenderTarget()->ColorTexture( 0 )->RawFrameSize();
}

} // bv
