#pragma once

#include "FFmpegUtils.h"

#define SCALE_FLAGS SWS_BICUBIC

namespace bv 
{

namespace avencoder
{

namespace 
{
//**************************************
//
static void log_packet(const AVFormatContext *, const AVPacket *)
{
    //AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
    //printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
    //       av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
    //       av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
    //       av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
    //       pkt->stream_index);
}
}

//**************************************
//
bool FFmpegUtils::add_stream                        ( OutputStream *ost, AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id )
{
    AVCodecContext *c;
    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
                avcodec_get_name(codec_id));
        return false;
    }
    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        return false;
    }
    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        return false;
    }
    ost->enc = c;

    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    return true;
}

//**************************************
//
::AVFrame *  FFmpegUtils::alloc_picture         ( enum AVPixelFormat pix_fmt, int width, int height )
{
    ::AVFrame *picture;
    int ret;
    picture = av_frame_alloc();
    if (!picture)
        return nullptr;
    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;
    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        return nullptr;
    }
    return picture;
}

//**************************************
//
::AVFrame * FFmpegUtils::alloc_audio_frame  ( enum AVSampleFormat sample_fmt, uint64_t channel_layout, int sample_rate, int nb_samples )
{
    ::AVFrame *frame = av_frame_alloc();
    int ret;
    if (!frame) {
        fprintf(stderr, "Error allocating an audio frame\n");
        return nullptr;
    }
    frame->format = sample_fmt;
    frame->channel_layout = channel_layout;
    frame->sample_rate = sample_rate;
    frame->nb_samples = nb_samples;
    if (nb_samples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            fprintf(stderr, "Error allocating an audio buffer\n");
            return nullptr;
        }
    }
    return frame;
}

//**************************************
//
bool FFmpegUtils::open_video                ( AVCodec * codec, OutputStream * ost, AVDictionary * opt_arg )
{
    int ret;
    AVCodecContext *c = ost->enc;
    AVDictionary *opt = NULL;
    av_dict_copy(&opt, opt_arg, 0);
    /* open the codec */
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        //fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
		return false;
    }
    /* allocate and init a re-usable frame */
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        return false;
    }
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_BGRA) {
        ost->tmp_frame = alloc_picture(AV_PIX_FMT_BGRA, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            return false;
        }
    }
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        return false;
    }

    return true;
}

//**************************************
//
bool FFmpegUtils::open_audio                ( AVCodec *codec, OutputStream * ost, AVDictionary * opt_arg )
{
    AVCodecContext *c;
    int nb_samples;
    int ret;
    AVDictionary * opt = nullptr;
    c = ost->enc;
    /* open it */
    av_dict_copy( &opt, opt_arg, 0 );
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        //fprintf(stderr, "Could not open audio codec: %s\n", av_err2str(ret));
        return false;
    }

    if (c->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
        nb_samples = 10000;
    else
        nb_samples = c->frame_size;
    ost->frame     = alloc_audio_frame(c->sample_fmt, c->channel_layout,
                                       c->sample_rate, nb_samples);
    ost->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, c->channel_layout,
                                       c->sample_rate, c->sample_rate / 25);
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        return false;
    }
    /* create resampler context */
    ost->swr_ctx = swr_alloc();
    if (!ost->swr_ctx) {
        fprintf(stderr, "Could not allocate resampler context\n");
        return false;
    }
    /* set options */
    av_opt_set_int       (ost->swr_ctx, "in_channel_count",   c->channels,       0);
    av_opt_set_int       (ost->swr_ctx, "in_sample_rate",     c->sample_rate,    0);
    av_opt_set_sample_fmt(ost->swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
    av_opt_set_int       (ost->swr_ctx, "out_channel_count",  c->channels,       0);
    av_opt_set_int       (ost->swr_ctx, "out_sample_rate",    c->sample_rate,    0);
    av_opt_set_sample_fmt(ost->swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);
    /* initialize the resampling context */
    if ((ret = swr_init(ost->swr_ctx)) < 0) {
        fprintf(stderr, "Failed to initialize the resampling context\n");
        return false;
    }

    return true;
}

//**************************************
//
int FFmpegUtils::write_frame                ( AVFormatContext * fmt_ctx, const AVRational * time_base, AVStream * st, AVPacket * pkt )
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;
    /* Write the compressed frame to the media file. */
    log_packet(fmt_ctx, pkt);
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

//**************************************
//
bool FFmpegUtils::fill_bgra_image           ( ::AVFrame * pict, bv::AVFrameConstPtr srcFrame )
{
    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally;
     * make sure we do not overwrite it here
     */
    int ret = av_frame_make_writable(pict);
    if (ret < 0)
        return false;

    auto numlines = srcFrame->m_desc.height;
    auto linesize = pict->linesize[ 0 ];

    for( SizeType i = 0; i < numlines; ++i )
        memcpy( pict->data[ 0 ] + i * linesize, srcFrame->m_videoData->Get() + ( numlines - i - 1) * linesize, linesize );

	//memcpy( pict->data[0], srcFrame->m_videoData->Get(), srcFrame->m_videoData->Size() );
    return true;
}

//**************************************
//
::AVFrame * FFmpegUtils::get_video_frame    ( OutputStream * ost, bv::AVFrameConstPtr bvFrame )
{
    AVCodecContext *c = ost->enc;
    /* check if we want to generate more frames */
    AVRational avr;
    avr.num = 1;
    avr.den = 1;
    //if (av_compare_ts(ost->next_pts, c->time_base,
    //                  (int64_t)STREAM_DURATION, avr) >= 0)
    //    return nullptr;
    if (c->pix_fmt != AV_PIX_FMT_BGRA) {
        /* as we only generate a YUV420P picture, we must convert it
         * to the codec pixel format if needed */
        if (!ost->sws_ctx) {
            ost->sws_ctx = sws_getContext(c->width, c->height,
                                          AV_PIX_FMT_BGRA,
                                          c->width, c->height,
                                          c->pix_fmt,
                                          SCALE_FLAGS, nullptr, nullptr, nullptr );
            if (!ost->sws_ctx) {
                fprintf(stderr,
                        "Could not initialize the conversion context\n");
                return nullptr;
            }
        }
        fill_bgra_image(ost->tmp_frame, bvFrame);
        sws_scale(ost->sws_ctx,
                  (const uint8_t * const *)ost->tmp_frame->data, ost->tmp_frame->linesize,
                  0, c->height, ost->frame->data, ost->frame->linesize);
    } else {
        fill_bgra_image(ost->frame, bvFrame);
    }
    ost->frame->pts = ost->next_pts++;
    return ost->frame;
}

//**************************************
//
bool FFmpegUtils::write_video_frame( AVFormatContext * oc, OutputStream * ost, bv::AVFrameConstPtr bvFrame )
{
    int ret;
    AVCodecContext *c;
    ::AVFrame *frame;
    AVPacket pkt = { 0 };
    c = ost->enc;
    frame = get_video_frame(ost, bvFrame);
    av_init_packet(&pkt);
    /* encode the image */

    ret = avcodec_send_frame( ost->enc, frame );

    if( ret == 0 )
        ret = avcodec_receive_packet( ost->enc, &pkt );
    else
        return false;

    if (ret < 0) {
        //fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
        return false;
    }

    ret = write_frame(oc, &c->time_base, ost->st, &pkt);

    if (ret < 0) {
        //fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
        return false;
    }
    return frame != nullptr;
}

//**************************************
//
::AVFrame * FFmpegUtils::get_audio_frame( OutputStream * ost, bv::AVFrameConstPtr bvFrame )
{
    ::AVFrame *frame = ost->tmp_frame;
    int16_t *q = ( int16_t* ) frame->data[ 0 ];

    assert( bvFrame->m_audioData->Size() <= frame->linesize[ 0 ] );
    memcpy( q, bvFrame->m_audioData->Get(), bvFrame->m_audioData->Size() );

    frame->pts = ost->next_pts;
    ost->next_pts += frame->nb_samples;
    return frame;
}

//**************************************
//
/*
* encode one audio frame and send it to the muxer
* return 1 when encoding is finished, 0 otherwise
*/
bool FFmpegUtils::write_audio_frame( AVFormatContext *oc, OutputStream *ost, bv::AVFrameConstPtr bvFrame )
{
    AVCodecContext * c = ost->enc;
    AVPacket pkt = { 0 }; // data and size must be 0;
    ::AVFrame *frame;
    int ret;
    int dst_nb_samples;
    av_init_packet( &pkt );

    frame = get_audio_frame( ost, bvFrame );
    if( frame )
    {
        /* convert samples from native format to destination codec format, using the resampler */
        /* compute destination number of samples */
        dst_nb_samples = (int)av_rescale_rnd( swr_get_delay( ost->swr_ctx, c->sample_rate ) + frame->nb_samples,
                                         c->sample_rate, c->sample_rate, AV_ROUND_UP );
        assert( dst_nb_samples == frame->nb_samples );
        /* when we pass a frame to the encoder, it may keep a reference to it
        * internally;
        * make sure we do not overwrite it here
        */
        ret = av_frame_make_writable( ost->frame );
        if( ret < 0 )
            return false;
        /* convert to destination format */
        ret = swr_convert( ost->swr_ctx,
                           ost->frame->data, ost->frame->nb_samples,
                           ( const uint8_t ** ) frame->data, frame->nb_samples );
        if( ret < 0 )
        {
            fprintf( stderr, "Error while converting\n" );
            return false;
        }

        AVRational avr;
        avr.num = 1;
        avr.den = c->sample_rate;

        frame = ost->frame;
        frame->pts = av_rescale_q( ost->samples_count, avr, c->time_base );
        ost->samples_count += dst_nb_samples;
    }

    ret = avcodec_send_frame( ost->enc, frame );

    if( ret == 0 )
        ret = avcodec_receive_packet( ost->enc, &pkt );
    else
        return false;

    if( ret < 0 )
    {
        //fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
        return false;
    }


    ret = write_frame( oc, &c->time_base, ost->st, &pkt );
    if( ret < 0 )
    {
        //fprintf( stderr, "Error while writing audio frame: %s\n",
        //         av_err2str( ret ) );
        return false;
    }

    return frame != nullptr;
}

//**************************************
//
void FFmpegUtils::close_stream      ( OutputStream * ost )
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    av_frame_free(&ost->tmp_frame);
    sws_freeContext(ost->sws_ctx);
    swr_free(&ost->swr_ctx);
}

//**************************************
//
bool FFmpegUtils::configure_video_codec_context( OutputStream * ost, int w, int h, int64_t bit_rate, int frame_rate, enum AVCodecID codec_id )
{
    AVCodecContext * c = ost->enc;
    c->codec_id = codec_id;
    c->bit_rate = bit_rate;
    /* Resolution must be a multiple of two. */
    c->width    = w;
    c->height   = h;
    /* timebase: This is the fundamental unit of time (in seconds) in terms
        * of which frame timestamps are represented. For fixed-fps content,
        * timebase should be 1/framerate and timestamp increments should be
        * identical to 1. */

    AVRational avr;
    avr.num = 1;
    avr.den = frame_rate;

    ost->st->time_base = avr;
    c->time_base       = ost->st->time_base;
    c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt       = AV_PIX_FMT_YUV420P;
    if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B-frames */
        c->max_b_frames = 2;
    }
    if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        /* Needed to avoid using macroblocks in which some coeffs overflow.
            * This does not happen with normal video, it just happens here as
            * the motion of the chroma plane does not match the luma plane. */
        c->mb_decision = 2;
    }

    return true;
}

//**************************************
//
bool FFmpegUtils::configure_audio_codec_context( OutputStream * ost, AVCodec * codec, int64_t bit_rate, int sample_rate )
{
    AVCodecContext * c = ost->enc;
    c->sample_fmt  = codec->sample_fmts ?
        codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
    c->bit_rate    = bit_rate;
    c->sample_rate = sample_rate;
    if (codec->supported_samplerates) {
        c->sample_rate = codec->supported_samplerates[0];
        for (auto i = 0; codec->supported_samplerates[i]; i++) {
            if (codec->supported_samplerates[i] == sample_rate)
                c->sample_rate = sample_rate;
        }
    }
    c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
    c->channel_layout = AV_CH_LAYOUT_STEREO;
    if (codec->channel_layouts) {
        c->channel_layout = codec->channel_layouts[0];
        for (auto i = 0; codec->channel_layouts[i]; i++) {
            if (codec->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                c->channel_layout = AV_CH_LAYOUT_STEREO;
        }
    }
    c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);

    AVRational avr;
	avr.num = 1;
	avr.den = c->sample_rate;
    ost->st->time_base = avr;

    return true;
}

} //avencoder
} // bv