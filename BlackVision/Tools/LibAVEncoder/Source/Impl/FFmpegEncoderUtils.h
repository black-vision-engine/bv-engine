#pragma once

#include "Util/FFmpeg/FFmpegDef.h"
#include "Memory/AVFrame.h"

namespace bv 
{

namespace avencoder
{

// a wrapper around a single output AVStream
struct OutputStream {
	::AVStream *st;
	AVCodecContext *enc;
	/* pts of the next frame that will be generated */
	int64_t next_pts;
	int samples_count;
	::AVFrame *frame;
	::AVFrame *tmp_frame;
	float t, tincr, tincr2;
	struct SwsContext *sws_ctx;
	struct SwrContext *swr_ctx;

    OutputStream()
        : st( nullptr )
        , enc( nullptr )
        , next_pts( 0 )
        , samples_count( 0 )
        , frame( nullptr )
        , tmp_frame( nullptr )
        , t( 0.f )
        , tincr( 0 )
        , tincr2( 0 )
        , sws_ctx( nullptr )
        , swr_ctx( nullptr )
    {}
};

struct FFmpegEncoderUtils
{
    
/* Add an output stream. */
static bool         add_stream      ( OutputStream * ost, AVFormatContext * oc, AVCodec ** codec, enum AVCodecID codec_id );
static ::AVFrame *  alloc_picture   ( enum AVPixelFormat pix_fmt, int width, int height );

static ::AVFrame *  alloc_audio_frame( enum AVSampleFormat sample_fmt, uint64_t channel_layout, int sample_rate, int nb_samples );
static bool         open_video      ( AVCodec * codec, OutputStream * ost, AVDictionary * opt_arg );
static bool         open_audio      ( AVCodec * codec, OutputStream * ost, AVDictionary * opt_arg );

static int          write_frame     ( AVFormatContext * fmt_ctx, const AVRational * time_base, AVStream * st, AVPacket * pkt );

static bool         fill_bgra_image (::AVFrame * pict, bv::AVFrameConstPtr srcFrame );

static ::AVFrame *  get_video_frame ( OutputStream * ost, bv::AVFrameConstPtr bvFrame );
static ::AVFrame *  get_audio_frame ( OutputStream * ost, bv::AVFrameConstPtr bvFrame );

static bool         write_video_frame( AVFormatContext * oc, OutputStream * ost, bv::AVFrameConstPtr bvFrame );
static bool         write_audio_frame( AVFormatContext * oc, OutputStream * ost, bv::AVFrameConstPtr bvFrame );

static void         close_stream( OutputStream * ost );

static bool         configure_video_codec_context( OutputStream * ost, int w, int h, int64_t bit_rate, int frame_rate, enum AVCodecID codec_id );

static bool         configure_audio_codec_context( OutputStream * ost, AVCodec * codec, int64_t bit_rate, int sample_rate );

};

} //avencoder
} // bv