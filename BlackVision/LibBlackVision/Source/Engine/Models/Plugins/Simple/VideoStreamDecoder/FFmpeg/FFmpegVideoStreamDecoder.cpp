#include "FFmpegVideoStreamDecoder.h"

#include <cassert>

namespace bv
{

// *******************************
//
FFmpegVideoStreamDecoder::FFmpegVideoStreamDecoder     ( AVFormatContext * formatCtx, Int32 streamIdx )
	: m_codecCtx( nullptr )
	, m_codec( nullptr )
	, m_swsCtx( nullptr )
	, m_width( 0 )
	, m_height( 0 )
	, m_frameRate( 0 )
	, m_currFrame( 0 )
	, m_streamIdx( streamIdx )
{
	m_codecCtx = formatCtx->streams[ streamIdx ]->codec;

	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
	assert( m_codec != nullptr );

	bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
	assert( !error ); { error; }

	//FIXME
	m_swsCtx = sws_getContext( 
		m_codecCtx->width,
		m_codecCtx->height,
		m_codecCtx->pix_fmt,
		m_codecCtx->width,
		m_codecCtx->height,
		AV_PIX_FMT_BGRA,
		SWS_BILINEAR,
		nullptr,
		nullptr,
		nullptr
	);

	m_width = ( UInt32 )m_codecCtx->width;
	m_height = ( UInt32 )m_codecCtx->height;
	m_frameRate = 25; //FIXME

	/*m_frame = av_frame_alloc();

	m_outFrame = new AVPicture();
	avpicture_alloc( m_outFrame, AV_PIX_FMT_BGRA, m_width, m_height );
	int size = avpicture_get_size( AV_PIX_FMT_BGRA, m_width, m_height );
	m_frameData = MemoryChunk::Create( ( char * )m_outFrame->data[ 0 ], SizeType( size ) );

	m_swsCtx = sws_getContext( m_codecCtx->width,
		m_codecCtx->height,
		m_codecCtx->pix_fmt,
		m_codecCtx->width,
		m_codecCtx->height,
		AV_PIX_FMT_BGRA,
		SWS_BILINEAR,
		nullptr,
		nullptr,
		nullptr
	);
	*/
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
	if ( frameReady )
	{
		m_currFrame++;
	}

    av_free_packet( packet );
    return ( frameReady != 0 );
}

// *******************************
//
void				FFmpegVideoStreamDecoder::ConvertFrame		( AVFrame * inFrame, AVFrame * outFrame )
{
	//FIXME
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
UInt32					FFmpegVideoStreamDecoder::GetFrameRate	() const
{
	return m_frameRate;
}

// *******************************
//
Int32					FFmpegVideoStreamDecoder::GetStreamIdx	() const
{
	return m_streamIdx;
}

// *******************************
//
UInt32					FFmpegVideoStreamDecoder::GetCurrentFrameId	() const
{
	return m_currFrame;
}

} //bv