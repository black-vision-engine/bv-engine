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
	m_demuxer = std::unique_ptr< FFmpegDemuxer >( new FFmpegDemuxer( path ) );
	m_vstreamDecoder = std::unique_ptr< FFmpegVideoStreamDecoder >( new FFmpegVideoStreamDecoder( m_demuxer->GetFormatContext(), m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO ) ) );

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
		m_decoderThread->Stop();
	}

	av_free( m_frame );
}

// *********************************
//
void						FFmpegVideoDecoder::Start				()
{
	if( m_decoderThread == nullptr )
	{
		m_decoderThread = std::unique_ptr< VideoDecoderThread >( new VideoDecoderThread( shared_from_this() ) );
		m_decoderThread->Start();
	}
	else
	{
		m_decoderThread->Resume();
	}
}

// *********************************
//
void						FFmpegVideoDecoder::Pause				()
{
	if( m_decoderThread != nullptr )
	{
		m_decoderThread->Pause();
	}
}

// *********************************
//
void						FFmpegVideoDecoder::Stop				()
{
	if( m_decoderThread != nullptr )
	{
		m_decoderThread->Stop();
		m_decoderThread = nullptr;

		m_demuxer->Seek( 0 );
		m_vstreamDecoder->Reset();
	}
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
		std::lock_guard< std::mutex > guard( m_dataMutex );
		if( m_vstreamDecoder->DecodePacket( packet, m_frame ) )
		{
			m_vstreamDecoder->ConvertFrame( m_frame, m_outFrame );

			int size = avpicture_get_size( static_cast< AVPixelFormat >( m_outFrame->format ), m_outFrame->width, m_outFrame->height );
			char * data = new char[ size ];
			memcpy( data, ( char * )m_outFrame->data[ 0 ], size );
			
			//reuse allocated memory?
			m_frameData = MemoryChunk::Create( data, SizeType( size ) );

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

// *********************************
//
UInt32						FFmpegVideoDecoder::GetDuration			() const
{
	return m_demuxer->GetDuration();

}

} //bv