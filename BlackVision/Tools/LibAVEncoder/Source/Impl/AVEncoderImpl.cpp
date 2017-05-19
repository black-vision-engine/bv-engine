#include "AVEncoderImpl.h"

#include <stdio.h>

#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define SCALE_FLAGS SWS_BICUBIC

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoder::Impl::Impl       ()
    : m_encoderThread( new AVEncoderThread() )
{
	m_encoderThread->Start();
}

namespace {

/* Add an output stream. */
static void add_stream(OutputStream *ost, AVFormatContext *oc,
                       AVCodec **codec,
                       enum AVCodecID codec_id)
{
    AVCodecContext *c;
    int i;
    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
                avcodec_get_name(codec_id));
        exit(1);
    }
    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }
    ost->enc = c;

	AVRational avr;

    switch ((*codec)->type) {
    case AVMEDIA_TYPE_AUDIO:
        c->sample_fmt  = (*codec)->sample_fmts ?
            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        c->bit_rate    = 64000;
        c->sample_rate = 44100;
        if ((*codec)->supported_samplerates) {
            c->sample_rate = (*codec)->supported_samplerates[0];
            for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                if ((*codec)->supported_samplerates[i] == 44100)
                    c->sample_rate = 44100;
            }
        }
        c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
        c->channel_layout = AV_CH_LAYOUT_STEREO;
        if ((*codec)->channel_layouts) {
            c->channel_layout = (*codec)->channel_layouts[0];
            for (i = 0; (*codec)->channel_layouts[i]; i++) {
                if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                    c->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
        c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
		avr.num = 1;
		avr.den = c->sample_rate;
        ost->st->time_base = avr;
        break;
    case AVMEDIA_TYPE_VIDEO:
        c->codec_id = codec_id;
        c->bit_rate = 400000;
        /* Resolution must be a multiple of two. */
        c->width    = 1920;
        c->height   = 1080;
        /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
         * identical to 1. */
		avr.num = 1;
		avr.den = STREAM_FRAME_RATE;

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
    break;
    default:
        break;
    }
    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

static ::AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    ::AVFrame *picture;
    int ret;
    picture = av_frame_alloc();
    if (!picture)
        return NULL;
    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;
    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }
    return picture;
}

/**************************************************************/
/* audio output */
static ::AVFrame *alloc_audio_frame(enum AVSampleFormat sample_fmt,
                                  uint64_t channel_layout,
                                  int sample_rate, int nb_samples)
{
    ::AVFrame *frame = av_frame_alloc();
    int ret;
    if (!frame) {
        fprintf(stderr, "Error allocating an audio frame\n");
        exit(1);
    }
    frame->format = sample_fmt;
    frame->channel_layout = channel_layout;
    frame->sample_rate = sample_rate;
    frame->nb_samples = nb_samples;
    if (nb_samples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            fprintf(stderr, "Error allocating an audio buffer\n");
            exit(1);
        }
    }
    return frame;
}

static void open_video(AVFormatContext *, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
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
		assert( false );
    }
    /* allocate and init a re-usable frame */
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_BGRA) {
        ost->tmp_frame = alloc_picture(AV_PIX_FMT_BGRA, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}

static void open_audio(AVFormatContext *, AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
{
    AVCodecContext *c;
    int nb_samples;
    int ret;
    AVDictionary *opt = NULL;
    c = ost->enc;
    /* open it */
    av_dict_copy(&opt, opt_arg, 0);
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        //fprintf(stderr, "Could not open audio codec: %s\n", av_err2str(ret));
        exit(1);
    }
    /* init signal generator */
    ost->t     = 0;
    ost->tincr = 2 * float(M_PI) * 110.f / c->sample_rate;
    /* increment frequency by 110 Hz per second */
    ost->tincr2 = 2 * float(M_PI) * 110.f / c->sample_rate / c->sample_rate;
    if (c->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
        nb_samples = 10000;
    else
        nb_samples = c->frame_size;
    ost->frame     = alloc_audio_frame(c->sample_fmt, c->channel_layout,
                                       c->sample_rate, nb_samples);
    ost->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, c->channel_layout,
                                       c->sample_rate, nb_samples);
    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
    /* create resampler context */
        ost->swr_ctx = swr_alloc();
        if (!ost->swr_ctx) {
            fprintf(stderr, "Could not allocate resampler context\n");
            exit(1);
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
            exit(1);
        }
}

static void log_packet(const AVFormatContext *, const AVPacket *)
{
    //AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;
    //printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
    //       av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
    //       av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
    //       av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
    //       pkt->stream_index);
}

static int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
{
    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;
    /* Write the compressed frame to the media file. */
    log_packet(fmt_ctx, pkt);
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

static void fill_bgra_image(::AVFrame *pict, bv::AVFramePtr srcFrame)
{
//	int x, y, i, ret;
    /* when we pass a frame to the encoder, it may keep a reference to it
     * internally;
     * make sure we do not overwrite it here
     */
    int ret = av_frame_make_writable(pict);
    if (ret < 0)
        exit(1);

	memcpy( pict->data[0], srcFrame->m_videoData->Get(), srcFrame->m_videoData->Size() );

    ///* Y */
    //for (y = 0; y < height; y++)
    //    for (x = 0; x < width; x++)
    //        pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
    ///* Cb and Cr */
    //for (y = 0; y < height / 2; y++) {
    //    for (x = 0; x < width / 2; x++) {
    //        pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
    //        pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
    //    }
    //}
}

static ::AVFrame *get_video_frame(OutputStream *ost, bv::AVFramePtr bvFrame)
{
    AVCodecContext *c = ost->enc;
    /* check if we want to generate more frames */
	AVRational avr;
	avr.num = 1;
	avr.den = 1;
    if (av_compare_ts(ost->next_pts, c->time_base,
                      (int64_t)STREAM_DURATION, avr) >= 0)
        return NULL;
    if (c->pix_fmt != AV_PIX_FMT_BGRA) {
        /* as we only generate a YUV420P picture, we must convert it
         * to the codec pixel format if needed */
        if (!ost->sws_ctx) {
            ost->sws_ctx = sws_getContext(c->width, c->height,
                                          AV_PIX_FMT_YUV420P,
                                          c->width, c->height,
                                          c->pix_fmt,
                                          SCALE_FLAGS, NULL, NULL, NULL);
            if (!ost->sws_ctx) {
                fprintf(stderr,
                        "Could not initialize the conversion context\n");
                exit(1);
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

static int write_video_frame(AVFormatContext *oc, OutputStream *ost, bv::AVFramePtr bvFrame)
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
		{
			return ( avcodec_receive_packet( ost->enc, &pkt ) == 0 );
		}
		else
		{
			assert(false);
		}

    //ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
    if (ret < 0) {
        //fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
        exit(1);
    }

        ret = write_frame(oc, &c->time_base, ost->st, &pkt);

    if (ret < 0) {
        //fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
        exit(1);
    }
    return (frame) ? 0 : 1;
}

}

//**************************************
//
bool            AVEncoder::Impl::OpenOutputStream       ( const std::string & outputFilePath, bool /*enableVideo*/, bool /*enableAudio*/ )
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
    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        add_stream(&m_video_st, m_AVContext, &video_codec, fmt->video_codec);
        have_video = 1;
        encode_video = 1;
    }
    if (fmt->audio_codec != AV_CODEC_ID_NONE) {
        add_stream(&m_audio_st, m_AVContext, &audio_codec, fmt->audio_codec);
        have_audio = 1;
        encode_audio = 1;
    }
    /* Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers. */
    if (have_video)
        open_video(m_AVContext, video_codec, &m_video_st, opt);
    if (have_audio)
        open_audio(m_AVContext, audio_codec, &m_audio_st, opt);
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

}

//**************************************
//
bool            AVEncoder::Impl::WriteFrame             ( const AVFramePtr & bvFrame )
{
	write_video_frame( m_AVContext, &m_video_st, bvFrame );
    return true;
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