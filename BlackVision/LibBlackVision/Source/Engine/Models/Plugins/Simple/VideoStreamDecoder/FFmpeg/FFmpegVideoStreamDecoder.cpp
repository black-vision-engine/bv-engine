#include "stdafx.h"

#include "FFmpegVideoStreamDecoder.h"

#include <cassert>
#include "FFmpegUtils.h"

namespace bv
{

// *******************************
//
FFmpegVideoStreamDecoder::FFmpegVideoStreamDecoder     ( VideoStreamAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx )
	: m_stream( nullptr )
	, m_codecCtx( nullptr )
	, m_codec( nullptr )
	, m_swsCtx( nullptr )
	, m_width( 0 )
	, m_height( 0 )
	, m_frameRate( 0 )
	, m_streamIdx( streamIdx )
{
	m_stream = formatCtx->streams[ streamIdx ];
	m_codecCtx = m_stream->codec;
	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
	assert( m_codec != nullptr );

	bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
	assert( !error ); { error; }

	//raw video desc should provide width, height & format

	if( m_codecCtx->width == 0 || m_codecCtx->height == 0 )
	{
		m_codecCtx->width = asset->GetWidth();
		m_codecCtx->height = asset->GetHeight();

		m_stream->avg_frame_rate = av_d2q( asset->GetFrameRate(), INT_MAX );
		m_stream->time_base = av_inv_q( m_stream->avg_frame_rate );

		m_codecCtx->pix_fmt = FFmpegUtils::ToFFmpegPixelFormat( asset->GetVideoFormat() );
	}

	m_width = ( UInt32 )m_codecCtx->width;
	m_height = ( UInt32 )m_codecCtx->height;
	
	m_frameRate = ( Float64 )av_q2d( m_stream->avg_frame_rate );

	assert( m_width > 0 );
	assert( m_height > 0 );

    auto ffmpegFormat = FFmpegUtils::ToFFmpegPixelFormat( asset->GetTextureFormat() );
    m_swsCtx = sws_getCachedContext( m_swsCtx, m_width, m_height, m_codecCtx->pix_fmt,
		m_width, m_height, ffmpegFormat, SWS_BILINEAR, nullptr, nullptr, nullptr );
}

// *******************************
//
FFmpegVideoStreamDecoder::~FFmpegVideoStreamDecoder    ()
{
	sws_freeContext( m_swsCtx );
	avcodec_close( m_codecCtx );
}

// *******************************
//
bool				FFmpegVideoStreamDecoder::DecodePacket		( AVPacket * packet, AVFrame * frame )
{
	assert( packet != nullptr );

	int frameReady = 0;
	avcodec_decode_video2( m_codecCtx, frame, &frameReady, packet );

    return ( frameReady != 0 );
}

// *******************************
//
void				FFmpegVideoStreamDecoder::ConvertFrame		( AVFrame * inFrame, AVFrame * outFrame )
{
	sws_scale( m_swsCtx, inFrame->data, inFrame->linesize, 0, inFrame->height, outFrame->data, outFrame->linesize );
}

// *******************************
//
UInt32					FFmpegVideoStreamDecoder::GetWidth	() const
{
	return m_width;
}

// *******************************
//
UInt32					FFmpegVideoStreamDecoder::GetHeight	() const
{
	return m_height;
}

// *******************************
//
Float64					FFmpegVideoStreamDecoder::GetFrameRate		() const
{
	return m_frameRate;
}

// *******************************
//
Int64					FFmpegVideoStreamDecoder::ConvertTime		( Float64 time )
{
    auto timeBase = av_q2d( m_stream->time_base );
	return ( Int64 )( time / timeBase );
}

// *******************************
//
void					FFmpegVideoStreamDecoder::Reset				()
{
	avcodec_flush_buffers( m_codecCtx );
	//m_codecCtx->frame_number = 0;
}

// *******************************
//
Int32					FFmpegVideoStreamDecoder::GetStreamIdx		() const
{
	return m_streamIdx;
}

// *******************************
//
UInt32					FFmpegVideoStreamDecoder::GetCurrentFrameId	() const
{
	return ( UInt32 )m_codecCtx->frame_number;
}

} //bv