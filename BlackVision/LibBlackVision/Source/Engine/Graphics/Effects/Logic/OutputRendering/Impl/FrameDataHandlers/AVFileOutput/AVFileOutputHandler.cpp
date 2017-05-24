#include "stdafx.h"

#include "AVFileOutputHandler.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/Logic/State/OutputState.h"

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
    , m_encoder( new avencoder::AVEncoder()  )
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

    m_encoder->OpenOutputStream( outputFilePath, vOps, avencoder::AudioOptions(), true, true );
}

// **************************
//
void AVFileOutputHandler::StopToAVFileRendering   ()
{
    if( m_encoder )
        m_encoder->CloseStream();
}


// **************************
//
AVFileOutputHandler::~AVFileOutputHandler     ()
{
    if( m_encoder )
        m_encoder->CloseStream();
    delete m_encoder;
}

// **************************
//
void                                AVFileOutputHandler::HandleFrameData     ( const OutputState & state, RenderContext * ctx, const RenderChannel * channel )
{
    //1. Prepare memory representation of current frame
    auto avFrame = PrepareFrame( state, ctx, channel );

    //2. Process memory representation of current frame
    ProcessFrame( avFrame );
}

// **************************
//
FullscreenEffectComponentStatePtr  AVFileOutputHandler::GetInternalFSEState       ()
{
    return nullptr;
}

// **************************
//
AVFrameConstPtr    AVFileOutputHandler::PrepareFrame                                  ( const OutputState &, RenderContext * ctx, const RenderChannel * inputChannel )
{
    Texture2DPtr outputFrame = ReadDefaultTexture( ctx, inputChannel );

    if( !m_avFrame )
    {
        m_avFrame = AVFrame::Create();
        m_avFrame->m_videoData = outputFrame->GetData();
    }

    //auto audio = audio_renderer( ctx );
    //auto ret = audio->GetBufferedData( std::const_pointer_cast< MemoryChunk >( avFrame->m_audioData ), vic->GetWrappedChannel()->AccessRenderChannelAudioEntities() );

    return m_avFrame;
}

// **************************
//
void            AVFileOutputHandler::ProcessFrame                                  ( AVFrameConstPtr frame )
{
    m_encoder->WriteFrame( frame );
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
