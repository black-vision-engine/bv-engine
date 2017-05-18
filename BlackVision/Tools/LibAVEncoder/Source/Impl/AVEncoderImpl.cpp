#include "AVEncoderImpl.h"



#include <stdio.h>

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoder::Impl::Impl       ()
    : m_encoderThread( new AVEncoderThread() )
{}

//**************************************
//
bool            AVEncoder::Impl::OpenOutputStream       ( const std::string & outputFilePath, bool /*enableVideo*/, bool /*enableAudio*/ )
{
    av_register_all();

    auto fmt = av_guess_format( nullptr, outputFilePath.c_str(), nullptr );
    if ( !fmt ) {
        //printf("Could not deduce output format from file extension: using MPEG.\n");
        fmt = av_guess_format( "mpeg", nullptr, nullptr );
    }
    if ( !fmt ) {
        //fprintf(stderr, "Could not find suitable output format\n");
        return false;
    }

    auto codec_id = fmt->video_codec;

    auto codec = avcodec_find_encoder( codec_id );
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    auto c = avcodec_alloc_context3(codec);
    m_AVContext = c;
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 352;
    c->height = 288;
    /* frames per second */
    AVRational ar = {1, 25};
    c->time_base = ar;
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_BGRA;
    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
     fopen_s(&m_file, outputFilePath.c_str(), "wb");
    if (!m_file) {
        fprintf(stderr, "Could not open %s\n", outputFilePath.c_str());
        exit(1);
    }
    m_AVFrame = av_frame_alloc();
    if (!m_AVFrame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    m_AVFrame->format = c->pix_fmt;
    m_AVFrame->width  = c->width;
    m_AVFrame->height = c->height;
    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    auto ret = av_image_alloc(m_AVFrame->data, m_AVFrame->linesize, c->width, c->height,
                         c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        exit(1);
    }


    assert( false );
    return false;
}

//**************************************
//
void            AVEncoder::Impl::CloseStream            ()
{

}

//**************************************
//
bool            AVEncoder::Impl::WriteFrame             ( const AVFramePtr & frame )
{
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;    // packet data will be allocated by the encoder
    pkt.size = 0;
    fflush(stdout);
    m_AVFrame->pts = frame->m_frameNum;
    /* encode the image */
    auto ret = avcodec_send_frame( m_AVContext, m_AVFrame );
    if (ret < 0) {
        fprintf(stderr, "Error encoding frame\n");
        return false;
    }
    if (avcodec_receive_packet( m_AVContext, &pkt ) == 0 ) {
        printf("Write frame %3d (size=%5d)\n", frame->m_frameNum, pkt.size);
        fwrite(pkt.data, 1, pkt.size, m_file);
        av_packet_unref(&pkt);
    } 

    return true;
}

//**************************************
//
AVEncoder::Impl::~Impl      () {}


}
}