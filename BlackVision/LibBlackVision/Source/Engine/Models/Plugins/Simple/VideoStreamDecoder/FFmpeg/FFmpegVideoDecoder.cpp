#include "FFmpegVideoDecoder.h"

#include "FFmpegDemuxer.h"
#include "FFmpegVideoStreamDecoder.h"

namespace bv
{

// *********************************
//
FFmpegVideoDecoder::FFmpegVideoDecoder		( VideoStreamAssetDescConstPtr desc )
	: m_decoderThread( nullptr )
{
	auto path = desc->GetStreamPath();
	m_demuxer = new FFmpegDemuxer( path );
	m_vstreamDecoder = new FFmpegVideoStreamDecoder( m_demuxer->GetFormatContext(), m_demuxer->GetVideoStreamIndex() );

	m_frame = av_frame_alloc();

	auto width = m_vstreamDecoder->GetWidth();
	auto height = m_vstreamDecoder->GetHeight();
	
	//FIXME format
	m_outFrame = av_frame_alloc();
	auto numBytes = avpicture_get_size( AV_PIX_FMT_BGRA, width, height );
	auto buffer = (uint8_t *)av_malloc( numBytes * sizeof( uint8_t ) );
	avpicture_fill( ( AVPicture * )m_outFrame, buffer, AV_PIX_FMT_BGRA, width, height );
	m_outFrame->width = width;
	m_outFrame->height = height;
	m_outFrame->format = ( int )AV_PIX_FMT_BGRA;
}

// *********************************
//
FFmpegVideoDecoder::~FFmpegVideoDecoder		()
{
	if( m_decoderThread )
	{
		m_decoderThread->Join();
	}

	av_free( m_frame );

	delete m_demuxer;
	delete m_vstreamDecoder;
}

// *********************************
//
void						FFmpegVideoDecoder::StartDecoding		()
{
	if( m_decoderThread == nullptr )
	{
		m_decoderThread = std::unique_ptr< VideoDecoderThread >( new VideoDecoderThread( this ) );
	}
	m_decoderThread->Start();
}

// *********************************
//
MemoryChunkConstPtr			FFmpegVideoDecoder::GetCurrentFrameData		( UInt32 & outFrameId ) const
{
	std::lock_guard< std::mutex > guard( m_dataMutex );
	outFrameId = m_vstreamDecoder->GetCurrentFrameId();
	return m_frameData;
}

// *********************************
//
bool						FFmpegVideoDecoder::GetNextFrameData		()
{
	auto packet = m_demuxer->GetPacket( m_vstreamDecoder->GetStreamIdx() );
	if( packet != nullptr )
	{
		if( m_vstreamDecoder->DecodePacket( packet, m_frame ) )
		{
			m_vstreamDecoder->ConvertFrame( m_frame, m_outFrame );

			int size = avpicture_get_size( static_cast< AVPixelFormat >( m_outFrame->format ), m_outFrame->width, m_outFrame->height );
			char * data = new char[ size ];
			memcpy( data, ( char * )m_outFrame->data[ 0 ], size );
			
			//reuse allocated memory?
			m_dataMutex.lock();
			m_frameData = MemoryChunk::Create( data, SizeType( size ) );
			m_dataMutex.unlock();

			return true;
		}
	}
	return false;
}

// *********************************
//
UInt32						FFmpegVideoDecoder::GetWidth				() const
{
	return m_vstreamDecoder->GetWidth();
}

// *********************************
//
UInt32						FFmpegVideoDecoder::GetHeight				() const
{
	return m_vstreamDecoder->GetHeight();

}

// *********************************
//
UInt32						FFmpegVideoDecoder::GetFrameRate			() const
{
	return m_vstreamDecoder->GetFrameRate();

}

} //bv