#include "stdafx.h"

#include "FFmpegVideoDecoder.h"

#include "FFmpegUtils.h"

namespace bv
{

const UInt32			FFmpegVideoDecoder::MAX_QUEUE_SIZE = 2;

// *********************************
//
FFmpegVideoDecoder::FFmpegVideoDecoder		( VideoStreamAssetConstPtr asset )
	: m_decoderThread( nullptr )
{
	auto path = asset->GetStreamPath();

	m_demuxer = std::unique_ptr< FFmpegDemuxer >( new FFmpegDemuxer( path ) );

	m_vstreamDecoder = std::unique_ptr< FFmpegVideoStreamDecoder >
		( new FFmpegVideoStreamDecoder( asset, m_demuxer->GetFormatContext(), m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO ) ) );

	m_frame = av_frame_alloc();

	auto width = m_vstreamDecoder->GetWidth();
	auto height = m_vstreamDecoder->GetHeight();
	
	auto ffmpegFormat = FFmpegUtils::ToFFmpegPixelFormat( asset->GetTextureFormat() );
	m_outFrame = av_frame_alloc();
	auto numBytes = avpicture_get_size( ffmpegFormat, width, height );
	m_outBuffer = ( uint8_t * )av_malloc( numBytes * sizeof( uint8_t ) );
	avpicture_fill( ( AVPicture * )m_outFrame, m_outBuffer, ffmpegFormat, width, height );
    
	m_outFrame->width = width;
	m_outFrame->height = height;
	m_outFrame->format = ( int )ffmpegFormat;
	m_frameSize = avpicture_get_size( static_cast< AVPixelFormat >( m_outFrame->format ), m_outFrame->width, m_outFrame->height );
}

// *********************************
//
FFmpegVideoDecoder::~FFmpegVideoDecoder		()
{
	if( m_decoderThread )
	{
		m_decoderThread->Stop();
		m_decoderThread->Join();
		m_decoderThread = nullptr;
	}

	//force 
	m_vstreamDecoder = nullptr;
	m_demuxer = nullptr;

	av_frame_free( &m_frame );
    av_free( m_outBuffer );

	std::lock_guard< std::mutex > lock( m_mutex );

	m_frameQueue.Clear();
}

// *********************************
//
void						FFmpegVideoDecoder::Start				()
{
	if( m_decoderThread == nullptr )
	{
		m_decoderThread = std::unique_ptr< VideoDecoderThread >( new VideoDecoderThread( this ) );
		m_decoderThread->Start();
	}
	else if ( m_decoderThread->Paused() )
    {
		m_decoderThread->Start();
    }
	else if ( m_decoderThread->Stopped() )
	{
		Reset();
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
		Reset();
	}
}

// *********************************
//
VideoMediaData			FFmpegVideoDecoder::GetVideoMediaData		()
{
	VideoMediaData mediaData;
	m_frameQueue.TryPop( mediaData );
	return mediaData;
}

// ***********************
// Jumps to frameTime. This function doesn't return to time before this function call.
VideoMediaData		    FFmpegVideoDecoder::GetSingleFrame  		( TimeType frameTime)
{
    Seek( frameTime );
    if( DecodeNextFrame() )
    {
        NextFrameDataReady();
        return GetVideoMediaData();
    }

	VideoMediaData mediaData;
	mediaData.frameIdx = 0;
	mediaData.frameData = nullptr;
    return mediaData;
}

// *********************************
//
void					FFmpegVideoDecoder::NextFrameDataReady		()
{
	std::lock_guard< std::mutex > lock( m_mutex );
	char * data = new char[ m_frameSize ];
	memcpy( data, ( char * )m_outFrame->data[ 0 ], m_frameSize );

	VideoMediaData mediaData;
	mediaData.frameIdx = m_vstreamDecoder->GetCurrentFrameId();
	mediaData.frameData = MemoryChunk::Create( data, SizeType( m_frameSize ) );
	m_frameQueue.Push( mediaData );
}

// *********************************
//
bool					FFmpegVideoDecoder::DecodeNextFrame			()
{
    auto success = false;

	std::lock_guard< std::mutex > lock( m_mutex );
	auto packet = m_demuxer->GetPacket( m_vstreamDecoder->GetStreamIdx() );
	if( packet != nullptr )
	{
		if( m_frameQueue.Size() < MAX_QUEUE_SIZE && 
			m_vstreamDecoder->DecodePacket( packet, m_frame ) )
		{
			m_vstreamDecoder->ConvertFrame( m_frame, m_outFrame );
			success = true;
		}
		
	}
	av_free_packet( packet );

	return success;
}

// *********************************
//
SizeType				FFmpegVideoDecoder::GetFrameSize			() const
{
	return m_frameSize;
}

// *********************************
//
UInt32					FFmpegVideoDecoder::GetWidth				() const
{
	return m_vstreamDecoder->GetWidth();
}

// *********************************
//
UInt32					FFmpegVideoDecoder::GetHeight				() const
{
	return m_vstreamDecoder->GetHeight();
}

// *********************************
//
Float64					FFmpegVideoDecoder::GetFrameRate			() const
{
	return m_vstreamDecoder->GetFrameRate();
}

// *********************************
//
void					FFmpegVideoDecoder::Seek					( Float64 time ) 
{
	std::lock_guard< std::mutex > lock( m_mutex );
    
    auto ts = m_vstreamDecoder->ConvertTime( time );
	m_demuxer->Seek( ts, m_vstreamDecoder->GetStreamIdx() );

    Int64 currTs = 0;
    while( currTs < ts )
    {
        auto packet = m_demuxer->GetPacket( m_vstreamDecoder->GetStreamIdx() );
        if( packet != nullptr )
        {
	        m_vstreamDecoder->DecodePacket( packet, m_frame );
            currTs = packet->dts;
        }
        else if( m_demuxer->IsEOF() )
        {
            break;
        }
        av_free_packet( packet );
    }

	m_frameQueue.Clear();
}

// *********************************
//
void					FFmpegVideoDecoder::Reset					() 
{
	std::lock_guard< std::mutex > lock( m_mutex );
	m_demuxer->Reset();
	m_vstreamDecoder->Reset();
	m_frameQueue.Clear();
}

// *********************************
//
bool					FFmpegVideoDecoder::IsEOF					() const
{
	std::lock_guard< std::mutex > lock( m_mutex );
	return m_demuxer->IsEOF();
}

} //bv