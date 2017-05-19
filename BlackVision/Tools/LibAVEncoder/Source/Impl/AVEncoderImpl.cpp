#include "AVEncoderImpl.h"

#include <stdio.h>

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoder::Impl::Impl       ()
    : m_encoderThread( new AVEncoderThread() )
{
	m_encoderThread->Start();
}

//**************************************
//
bool            AVEncoder::Impl::OpenOutputStream       ( const std::string & outputFilePath, VideoOptions vOps, AudioOptions /*aOps*/, bool enableVideo, bool enableAudio )
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
        FFmpegUtils::configure_video_codec_context( m_video_st.enc, &m_video_st, vOps.width, vOps.height, vOps.bitRate, vOps.frameRate, fmt->video_codec );
        have_video = 1;
        encode_video = 1;
    }
    if (enableAudio && fmt->audio_codec != AV_CODEC_ID_NONE) {
        FFmpegUtils::add_stream(&m_audio_st, m_AVContext, &audio_codec, fmt->audio_codec);
        have_audio = 1;
        encode_audio = 1;
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

	return true;
}

//**************************************
//
void            AVEncoder::Impl::CloseStream            ()
{
    av_write_trailer(m_AVContext);
        /* Close each codec. */
   
    FFmpegUtils::close_stream(&m_video_st);
    //if (have_audio)
    //    close_stream(oc, &audio_st);
    if (!(m_AVContext->oformat->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_closep(&m_AVContext->pb);
    /* free the stream */
    avformat_free_context(m_AVContext);
}

//**************************************
//
bool            AVEncoder::Impl::WriteFrame             ( const AVFrameConstPtr & bvFrame )
{
	return FFmpegUtils::write_video_frame( m_AVContext, &m_video_st, bvFrame );
}

//**************************************
//
AVEncoder::Impl::~Impl      () 
{
	m_encoderThread->Kill();
	m_encoderThread->Join();
}


}
}