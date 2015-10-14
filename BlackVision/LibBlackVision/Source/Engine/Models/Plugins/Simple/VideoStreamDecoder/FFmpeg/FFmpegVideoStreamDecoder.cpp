#include "FFmpegVideoStreamDecoder.h"

#include <cassert>
#include "FFmpegUtils.h"

namespace bv
{

// *******************************
//
FFmpegVideoStreamDecoder::FFmpegVideoStreamDecoder     ( VideoStreamAssetDescConstPtr desc, AVFormatContext * formatCtx, Int32 streamIdx )
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
		m_codecCtx->width = desc->GetWidth();
		m_codecCtx->height = desc->GetHeight();

		m_codecCtx->framerate = av_d2q( desc->GetFrameRate(), ( int )desc->GetFrameRate() );
		m_stream->avg_frame_rate = m_codecCtx->framerate;

		m_codecCtx->pix_fmt = FFmpegUtils::ToFFmpegPixelFormat( desc->GetVideoFormat() );
	}

	m_width = ( UInt32 )m_codecCtx->width;
	m_height = ( UInt32 )m_codecCtx->height;
	
	m_frameRate = ( Float64 )av_q2d( m_stream->avg_frame_rate );

	assert( m_width > 0 );
	assert( m_height > 0 );

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

    av_free_packet( packet );
    return ( frameReady != 0 );
}

// *******************************
//
void				FFmpegVideoStreamDecoder::ConvertFrame		( AVFrame * inFrame, AVFrame * outFrame )
{
	m_swsCtx = sws_getCachedContext( m_swsCtx, inFrame->width, inFrame->height, static_cast< AVPixelFormat >( inFrame->format ),
		outFrame->width, outFrame->height, static_cast< AVPixelFormat >( outFrame->format ), SWS_BILINEAR, nullptr, nullptr, nullptr );
	
	sws_scale( m_swsCtx, ( const uint8_t ** const )inFrame->data, inFrame->linesize, 0,	inFrame->height, outFrame->data, outFrame->linesize );
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