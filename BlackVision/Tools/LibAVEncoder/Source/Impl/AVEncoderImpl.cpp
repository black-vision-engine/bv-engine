#include "AVEncoderImpl.h"

#include <stdio.h>

#include <functional>

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoder::Impl::Impl       ( UInt32 frameBufferSize )
    : m_encoderThread( nullptr )
    , m_frameBufferSize( frameBufferSize )
{}

//**************************************
//
bool            AVEncoder::Impl::OpenOutputStream       ( const std::string & outputFilePath, VideoOptions vOps, AudioOptions aOps, bool enableVideo, bool enableAudio )
{
	auto have_video = 0;
	auto have_audio	 = 0;
	int encode_video = 0, encode_audio = 0;
	AVCodec * audio_codec = nullptr;
	AVCodec * video_codec = nullptr;
	//m_video_st
	//m_audio_st = { 0 };
	AVDictionary *opt = NULL;
	int ret;

    av_register_all();

    /* allocate the output media context */
    avformat_alloc_output_context2( &m_AVContext, NULL, NULL, outputFilePath.c_str());
    if (!m_AVContext) {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&m_AVContext, NULL, "mpeg", outputFilePath.c_str());
    }
    if (!m_AVContext)
        return false;

    auto fmt = m_AVContext->oformat;
    /* Add the audio and video streams using the default format codecs
     * and initialize the codecs. */
    if (enableVideo && fmt->video_codec != AV_CODEC_ID_NONE) {
        FFmpegUtils::add_stream(&m_video_st, m_AVContext, &video_codec, fmt->video_codec);
        if( FFmpegUtils::configure_video_codec_context( &m_video_st, vOps.width, vOps.height, vOps.bitRate, vOps.frameRate, fmt->video_codec ) )
        {
            have_video = 1;
            encode_video = 1;
        }
    }
    if (enableAudio && fmt->audio_codec != AV_CODEC_ID_NONE) {
        FFmpegUtils::add_stream(&m_audio_st, m_AVContext, &audio_codec, fmt->audio_codec);
        if( FFmpegUtils::configure_audio_codec_context( &m_audio_st, audio_codec, aOps.bitRate, aOps.sampleRate ) )
        {
            have_audio = 1;
            encode_audio = 1;
        }
    }
    /* Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers. */
    if (have_video)
        FFmpegUtils::open_video(video_codec, &m_video_st, opt);
    if (have_audio)
        FFmpegUtils::open_audio(audio_codec, &m_audio_st, opt);
    av_dump_format(m_AVContext, 0, outputFilePath.c_str(), 1);
    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&m_AVContext->pb, outputFilePath.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            //fprintf(stderr, "Could not open '%s': %s\n", outputFilePath.c_str(),
            //        av_err2str(ret));
            return false;
        }
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(m_AVContext, &opt);
    if (ret < 0) {
        //fprintf(stderr, "Error occurred when opening output file: %s\n",
        //        av_err2str(ret));
        return false;
    }

    m_avFramesBuffer = boost::circular_buffer< AVFramePtr >( m_frameBufferSize );

    for( UInt32 i = 0; i < m_frameBufferSize; ++i )
    {
        auto videoData = bv::MemoryChunk::Create( 4 * vOps.width * vOps.height );

        bv::AVFrameDescriptor desc;
        desc.channels = aOps.numChannels;
        desc.height = vOps.height;
        desc.width = vOps.width;
        desc.sampleRate = aOps.sampleRate;

        auto frame = bv::AVFrame::Create(); // TODO: Add audio data
        frame->m_videoData = videoData;
        frame->m_desc = desc;

        m_avFramesBuffer.push_back( frame );
    }

    std::function<void( const AVFrameConstPtr & frame )> f = [ = ] ( const AVFrameConstPtr & frame )
    {
        this->FrameWritten( frame );
    };

    m_encoderThread = std::unique_ptr< AVEncoderThread >( new AVEncoderThread( m_AVContext, &m_video_st, &m_audio_st, f ) );
    m_encoderThread->Start();

	return true;
}

//**************************************
//
void            AVEncoder::Impl::CloseStream            ()
{
    if( m_encoderThread )
    {
        while( !m_encoderThread->IsEmpty() );
        m_encoderThread->Kill();
        m_encoderThread->EnqueueFrame( nullptr );
        m_encoderThread->Join();
        m_encoderThread = nullptr;


        av_write_trailer( m_AVContext );
        /* Close each codec. */

        FFmpegUtils::close_stream( &m_video_st );
        //if (have_audio)
        //    close_stream(oc, &audio_st);
        if( !( m_AVContext->oformat->flags & AVFMT_NOFILE ) )
            /* Close the output file. */
            avio_closep( &m_AVContext->pb );
        /* free the stream */
        avformat_free_context( m_AVContext );
    }
}

//**************************************
//
AVFramePtr      AVEncoder::Impl::GetFrameBuffer         ()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    if( !m_avFramesBuffer.empty() )
    {
        auto frame = m_avFramesBuffer.front();
        m_avFramesBuffer.pop_front();
        return frame;
    }
    else
        return nullptr;

}

//**************************************
//
void            AVEncoder::Impl::FrameWritten           ( const AVFrameConstPtr & frame )
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_avFramesBuffer.push_back( std::const_pointer_cast< AVFrame >( frame ) );
}

//**************************************
//
bool            AVEncoder::Impl::WriteFrame             ( const AVFrameConstPtr & bvFrame )
{
    AVFramePtr frame = GetFrameBuffer();

    if( !frame )
    {
        auto videoData = bv::MemoryChunk::Create( bvFrame->m_videoData->Size() );

        frame = bv::AVFrame::Create(); // TODO: Add audio data
        frame->m_videoData = videoData;
        frame->m_desc = bvFrame->m_desc;
    }

    memcpy( std::const_pointer_cast< MemoryChunk >( frame->m_videoData )->GetWritable(), bvFrame->m_videoData->Get(), bvFrame->m_videoData->Size() );

    m_encoderThread->EnqueueFrame( frame );

    return true;
}

//**************************************
//
AVEncoder::Impl::~Impl      () 
{
    if( m_encoderThread )
    {
        m_encoderThread->Kill();
        m_encoderThread->EnqueueFrame( nullptr );    
        m_encoderThread->Join();
    }
}


}
}