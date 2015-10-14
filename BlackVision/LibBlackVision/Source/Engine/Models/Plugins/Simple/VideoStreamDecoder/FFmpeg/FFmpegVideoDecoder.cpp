#include "FFmpegVideoDecoder.h"

#include "FFmpegUtils.h"

namespace bv
{

// *********************************
//
FFmpegVideoDecoder::FFmpegVideoDecoder		( VideoStreamAssetDescConstPtr desc )
	: m_decoderThread( nullptr )
{
	auto path = desc->GetStreamPath();

	m_demuxer = std::unique_ptr< FFmpegDemuxer >( new FFmpegDemuxer( path ) );

	m_vstreamDecoder = std::unique_ptr< FFmpegVideoStreamDecoder >
		( new FFmpegVideoStreamDecoder( desc, m_demuxer->GetFormatContext(), m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO ) ) );

	m_frame = av_frame_alloc();

	auto width = m_vstreamDecoder->GetWidth();
	auto height = m_vstreamDecoder->GetHeight();
	
	auto ffmpegFormat = FFmpegUtils::ToFFmpegPixelFormat( desc->GetTextureFormat() );
	m_outFrame = av_frame_alloc();
	auto numBytes = avpicture_get_size( ffmpegFormat, width, height );
	auto buffer = ( uint8_t * )av_malloc( numBytes * sizeof( uint8_t ) );
	avpicture_fill( ( AVPicture * )m_outFrame, buffer, ffmpegFormat, width, height );
	
	m_outFrame->width = width;
	m_outFrame->height = height;
	m_outFrame->format = ( int )ffmpegFormat;
	m_frameSize = avpicture_get_size( static_cast< AVPixelFormat >( m_outFrame->format ), m_outFrame->width, m_outFrame->height );

	ClearFrameData();
}

// *********************************
//
FFmpegVideoDecoder::~FFmpegVideoDecoder		()
{
	if( m_decoderThread )
	{
		m_decoderThread->Stop();
		m_decoderThread = nullptr;
	}

	av_free( m_frame );
}

// *********************************
//
void						FFmpegVideoDecoder::Start				()
{
	if( m_decoderThread == nullptr || m_decoderThread->Stopped() )
	{
		Reset();
		m_decoderThread = std::unique_ptr< VideoDecoderThread >( new VideoDecoderThread( shared_from_this() ) );
		m_decoderThread->Start();
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

		Reset();
	}
}

// *********************************
//
MemoryChunkConstPtr			FFmpegVideoDecoder::GetCurrentFrameData		( UInt64 & outFrameId ) const
{
	std::lock_guard< std::mutex > guard( m_dataMutex );
	outFrameId = m_vstreamDecoder->GetCurrentFrameId();
	return m_frameData;
}

// *********************************
//
bool						FFmpegVideoDecoder::NextFrameDataReady		()
{
	std::lock_guard< std::mutex > guard( m_dataMutex );
	auto packet = m_demuxer->GetPacket( m_vstreamDecoder->GetStreamIdx() );
	if( packet != nullptr )
	{
		if( m_vstreamDecoder->DecodePacket( packet, m_frame ) )
		{
			m_vstreamDecoder->ConvertFrame( m_frame, m_outFrame );

			char * data = new char[ m_frameSize ];
			memcpy( data, ( char * )m_outFrame->data[ 0 ], m_frameSize );
			m_frameData = MemoryChunk::Create( data, SizeType( m_frameSize ) );

			return true;
		}
	}
	return false;
}

// *********************************
//
SizeType					FFmpegVideoDecoder::GetFrameSize			() const
{
	return m_frameSize;
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
Float64						FFmpegVideoDecoder::GetFrameRate			() const
{
	return m_vstreamDecoder->GetFrameRate();
}

// *********************************
//
void						FFmpegVideoDecoder::Reset					() 
{
	m_demuxer->Reset();
	ClearFrameData();
}

// *********************************
//
bool						FFmpegVideoDecoder::IsEOF					() const
{
	std::lock_guard< std::mutex > guard( m_dataMutex );
	return m_demuxer->IsEOF();
}

// *********************************
//
void						FFmpegVideoDecoder::ClearFrameData			() 
{
	char * rawData = new char[ m_frameSize ];
	memset( rawData, 0, m_frameSize );
	m_frameData = MemoryChunk::Create( rawData, m_frameSize );
}

} //bv