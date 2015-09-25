#include "VideoStreamDecoderThread.h"
#include <Windows.h>

namespace bv
{

// *******************************
//
VideoStreamDecoderThread::VideoStreamDecoderThread     ( const std::string & streamPath )
	: m_formatCtx( nullptr )
	, m_codecCtx( nullptr )
	, m_codec( nullptr )
	, m_videoStreamIdx( -1 )
	, m_width( 0 )
	, m_height( 0 )
	, m_frameId( 0 )
{
	av_register_all();

	bool error = false; 
	
	error = avformat_open_input( &m_formatCtx, streamPath.c_str(), nullptr, nullptr ) != 0;
	
	assert( !error );

	av_dump_format( m_formatCtx, 0, streamPath.c_str(), 0 );

	for ( unsigned int i = 0; i < m_formatCtx->nb_streams; ++i )
	{
		if ( m_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && m_videoStreamIdx < 0 )
		{
			m_videoStreamIdx = ( Int32 )i;
		}
	}

	assert( m_videoStreamIdx > -1 );

	m_codecCtx = m_formatCtx->streams[ m_videoStreamIdx ]->codec;

	error = avformat_find_stream_info( m_formatCtx, nullptr ) < 0;

	assert( !error );

	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );

	assert( m_codec != nullptr );

	error = avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0;

	assert( !error );

	m_width = ( UInt32 )m_codecCtx->width;
	m_height = ( UInt32 )m_codecCtx->height;

	m_frame = av_frame_alloc();

	m_outFrame = new AVPicture();
	avpicture_alloc( m_outFrame, AV_PIX_FMT_RGBA, m_width, m_height );
	int size = avpicture_get_size( AV_PIX_FMT_RGBA, m_width, m_height );
	m_frameData = MemoryChunk::Create( ( char * )m_outFrame->data[ 0 ], SizeType( size ) );

	m_swsCtx = sws_getContext( m_codecCtx->width,
		m_codecCtx->height,
		m_codecCtx->pix_fmt,
		m_codecCtx->width,
		m_codecCtx->height,
		AV_PIX_FMT_RGBA,
		SWS_BILINEAR,
		nullptr,
		nullptr,
		nullptr
	);
}

// *******************************
//
VideoStreamDecoderThread::~VideoStreamDecoderThread    ()
{
	av_free( m_frame );
	av_free( m_outFrame );
	avcodec_close( m_codecCtx );

	avformat_close_input( &m_formatCtx );
	sws_freeContext( m_swsCtx ); 
}

// *******************************
//
void				VideoStreamDecoderThread::Run			()
{
	AVPacket packet;

	while( av_read_frame( m_formatCtx, &packet ) >= 0 )
	{
		if ( packet.stream_index == m_videoStreamIdx )
		{
			avcodec_decode_video2( m_codecCtx, m_frame, &m_frameReady, &packet );

			//double pts = packet.dts;
			//pts *= av_q2d( m_formatCtx->streams[ m_videoStreamIdx ]->time_base );
					
			if ( m_frameReady )
			{
				sws_scale( m_swsCtx, ( const uint8_t** const )m_frame->data, m_frame->linesize, 0, m_codecCtx->height, m_outFrame->data, m_outFrame->linesize );
				m_frameId++;
			
				av_free_packet(&packet);

				Sleep( 33 );
			}

		}
		else
		{
			av_free_packet(&packet);
		}
	}

}

// *******************************
//
UInt32					VideoStreamDecoderThread::GetWidth	() const
{
	return m_width;
}

// *******************************
//
UInt32					VideoStreamDecoderThread::GetHeight	() const
{
	return m_height;
}

// *******************************
//
MemoryChunkConstPtr		VideoStreamDecoderThread::GetFrameData			( UInt32 & outFrameId ) const
{
	outFrameId = m_frameId;
	return m_frameData;
}

// *******************************
//
bool					VideoStreamDecoderThread::NewVideoFrameArrived	( UInt32 lastFrameId ) const
{
	return m_frameId > lastFrameId;
}

} //bv
