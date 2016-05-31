#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"

#include <cassert>
#include "FFmpegUtils.h"

namespace bv
{

// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( VideoStreamAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx )
	: m_stream( nullptr )
	, m_codecCtx( nullptr )
	, m_codec( nullptr )
	, m_streamIdx( streamIdx )
{
	m_stream = formatCtx->streams[ streamIdx ];
	m_codecCtx = m_stream->codec;
	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
	assert( m_codec != nullptr );
    
	bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
	assert( !error ); { error; }

    m_sampleRate = m_codecCtx->sample_rate;
    m_channelsNum = m_codecCtx->channels;
    m_channelLayout = m_codecCtx->channel_layout;
}

// *******************************
//
FFmpegAudioStreamDecoder::~FFmpegAudioStreamDecoder    ()
{
	avcodec_close( m_codecCtx );
}

// *******************************
//
Int32				FFmpegAudioStreamDecoder::DecodePacket		( AVPacket * packet, AVFrame * frame )
{
	assert( packet != nullptr );

	int frameReady = 0;
    auto decoded = avcodec_decode_audio4( m_codecCtx, frame, &frameReady, packet );

    return std::min( decoded, packet->size );
}

// *******************************
//
void				FFmpegAudioStreamDecoder::ConvertFrame		( AVFrame * inFrame, AVFrame * outFrame )
{
    inFrame; outFrame;
}

// *******************************
//
Int64					FFmpegAudioStreamDecoder::ConvertTime		( Float64 time )
{
    auto timeBase = av_q2d( m_stream->time_base );
	return ( Int64 )( time / timeBase );
}

// *******************************
//
void					FFmpegAudioStreamDecoder::Reset				()
{
	avcodec_flush_buffers( m_codecCtx );
	m_codecCtx->frame_number = 0;
}

// *******************************
//
Int32					FFmpegAudioStreamDecoder::GetStreamIdx		() const
{
	return m_streamIdx;
}

} //bv