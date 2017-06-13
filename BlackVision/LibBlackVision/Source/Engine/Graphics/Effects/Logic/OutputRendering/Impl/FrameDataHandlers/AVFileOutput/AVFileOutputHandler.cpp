#include "stdafx.h"

#include "AVFileOutputHandler.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/Logic/State/OutputState.h"

#include "Engine/Audio/Resources/AudioMixer.h"
#include "Engine/Audio/Resources/AudioUtils.h"

#include "System/Time.h"

#include "AVEncoder.h"

namespace bv { 
   
// **************************
//
AVFileOutputHandler::AVFileOutputHandler      ( unsigned int width, unsigned int height )
    : m_width( width )
    , m_height( height )
    , m_activeRenderOutput( 1 )
    , m_shmRT( nullptr )
    , m_shmTexture( nullptr )
    , m_encoder( new avencoder::AVEncoder() )
    , m_audioMixer( new audio::AudioMixer() )
    , m_lastFrameTime( 0 )
{

}

// **************************
//
void AVFileOutputHandler::StartToAVFileRendering  ( const std::string & outputFilePath )
{
    avencoder::VideoOptions vOps;
    vOps.frameRate = 25;
    vOps.width = 1920;
    vOps.height = 1080;
    vOps.bitRate = 40000;

    m_fps = 25;

    avencoder::AudioOptions aOps;
    aOps.numChannels = 2;
    aOps.sampleRate = 44100;
    aOps.bitRate = 64000;
    aOps.sampleType = AudioSampleType::AV_SAMPLE_FMT_S16;

    m_avFrame = AVFrame::Create();
    m_avFrame->m_audioData = MemoryChunk::Create( audio::AudioUtils::AudioDataSize( 48000, aOps.numChannels, ConvertAudioSampleTypeToSampleSize( aOps.sampleType ), m_fps ) );

    m_encoder->OpenOutputStream( outputFilePath, vOps, aOps, true, true );
}

// **************************
//
void AVFileOutputHandler::StopToAVFileRendering   ()
{
    if( m_encoder )
        m_encoder->CloseStream();

    m_avFrame = nullptr;
}


// **************************
//
AVFileOutputHandler::~AVFileOutputHandler     ()
{
    if( m_encoder )
        m_encoder->CloseStream();
    delete m_encoder;

    delete m_audioMixer;
}

// **************************
//
void                                AVFileOutputHandler::HandleFrameData     ( const OutputState & state, RenderContext * ctx, const RenderChannel * channel )
{
    //1. Prepare memory representation of current frame
    PrepareFrame( state, ctx, channel );

    //2. Process memory representation of current frame
    ProcessFrame( ctx, channel );
}

// **************************
//
FullscreenEffectComponentStatePtr  AVFileOutputHandler::GetInternalFSEState       ()
{
    return nullptr;
}

// **************************
//
void            AVFileOutputHandler::PrepareFrame                                  ( const OutputState &, RenderContext * ctx, const RenderChannel * inputChannel )
{
    m_audioMixer->ResizeSources( inputChannel->GetAudioRenderChannelData().NumSources() );

    for( SizeType i = 0; i < inputChannel->GetAudioRenderChannelData().NumSources(); ++i )
        m_audioMixer->PushData( i, inputChannel->GetAudioRenderChannelData().GetData( i ) );

    m_audioMixer->SetGain( audio_renderer( ctx )->Gain() );
}

// **************************
//
void            AVFileOutputHandler::ProcessFrame                                  ( RenderContext * ctx, const RenderChannel * inputChannel )
{
    auto now = Time::Now();

    if( m_lastFrameTime == 0 )
        m_lastFrameTime = now;
    else
    {
        if( now - m_lastFrameTime >= 1000 / m_fps )
        {
            Texture2DPtr outputFrame = ReadDefaultTexture( ctx, inputChannel );

            m_avFrame->m_videoData = outputFrame->GetData();

            m_audioMixer->PopAndMixAudioData( std::const_pointer_cast< MemoryChunk >( m_avFrame->m_audioData ) );
            m_encoder->WriteFrame( m_avFrame );
            m_lastFrameTime = now;
        }
    }
    
}

// **************************
//
Texture2DPtr    AVFileOutputHandler::ReadDefaultTexture                            ( RenderContext * ctx, const RenderChannel * inputChannel )
{
	return inputChannel->ReadColorTexture( renderer( ctx ) );
}

//// **************************
//// FIXME: implement is as a part of some readback cache
//Texture2DPtr    AVFileOutputHandler::ReadMixChannelsTexture                        ( RenderContext * ctx, const RenderTarget * inputRenderTarget )
//{
//    if( m_shmRT == nullptr )
//    {
//        m_shmRT = allocator( ctx )->CreateCustomRenderTarget( m_width, m_height, RenderTarget::RTSemantic::S_DRAW_READ );
//    }
//
//    m_activeRenderOutput.SetEntry( 0, inputRenderTarget );
//    m_mixChannelsEffect->Render( ctx, m_shmRT, m_activeRenderOutput );
//
//    renderer( ctx )->ReadColorTexture( 0, m_shmRT, m_shmTexture );
//
//    return m_shmTexture;
//}


} //bv
