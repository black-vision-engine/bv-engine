#include "stdafx.h"

#include "FFmpegVideoDecoder.h"

#include "FFmpegUtils.h"

namespace bv
{

const UInt32			FFmpegVideoDecoder::MAX_QUEUE_SIZE = 25;

// *********************************
//
FFmpegVideoDecoder::FFmpegVideoDecoder		( VideoStreamAssetConstPtr asset )
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
	auto numBytes = av_image_get_buffer_size( ffmpegFormat, width, height, 1 );
	m_outBuffer = ( uint8_t * )av_malloc( numBytes * sizeof( uint8_t ) );
    av_image_fill_arrays( ( uint8_t ** ) m_outFrame->data, m_outFrame->linesize, m_outBuffer, ffmpegFormat, width, height, 1 );
	m_outFrame->width = width;
	m_outFrame->height = height;
	m_outFrame->format = ( int )ffmpegFormat;
	m_frameSize = av_image_get_buffer_size( static_cast< AVPixelFormat >( m_outFrame->format ), m_outFrame->width, m_outFrame->height, 1 );

	m_decoderThread = std::unique_ptr< VideoDecoderThread >( new VideoDecoderThread( this ) );
    m_decoderThread->Start();
}

// *********************************
//
FFmpegVideoDecoder::~FFmpegVideoDecoder		()
{
    assert( m_decoderThread );
	
	m_decoderThread->Kill();
	m_decoderThread->Join();
	m_decoderThread = nullptr;

	//force 
	m_vstreamDecoder = nullptr;
	m_demuxer = nullptr;

	av_frame_free( &m_frame );
    av_free( m_outBuffer );

	std::lock_guard< std::mutex > lock( m_mutex );

	m_outQueue.Clear();
	m_bufferQueue.Clear();
}

// *********************************
//
void						FFmpegVideoDecoder::Play				()
{
    assert( m_decoderThread );

    m_decoderThread->Play();
}

// *********************************
//
void						FFmpegVideoDecoder::Pause				()
{
    assert( m_decoderThread );
	
    m_decoderThread->Pause();
}

// *********************************
//
void						FFmpegVideoDecoder::Stop				()
{
    assert( m_decoderThread );
	
    m_decoderThread->Stop();
	Reset();
}

// *********************************
//
VideoMediaData			FFmpegVideoDecoder::GetVideoMediaData		()
{
	VideoMediaData mediaData;
    m_outQueue.Front( mediaData );
    return mediaData;
}

// ***********************
// Jumps to frameTime. This function doesn't return to time before this function call.
VideoMediaData		    FFmpegVideoDecoder::GetSingleFrame  		( TimeType frameTime )
{
    Seek( frameTime );
    FlushBuffers();
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
bool					FFmpegVideoDecoder::NextFrameDataReady		()
{
    VideoMediaData mediaData;
    if( m_bufferQueue.TryPop( mediaData ) )
    {
        m_outQueue.Push( mediaData );

        VideoMediaData del;
        m_outQueue.TryPop( del );

        return true;
    }

    return false;
}

// *********************************
//
bool					FFmpegVideoDecoder::DecodeNextFrame			()
{
    auto success = false;

    if( m_bufferQueue.Size() < GetMaxBufferSize() )
    {
	    std::lock_guard< std::mutex > lock( m_mutex );
	    auto packet = m_demuxer->GetPacket( m_vstreamDecoder->GetStreamIdx() );
	    if( packet )
	    {
            if( m_vstreamDecoder->DecodePacket( packet, m_frame ) )
            {
		        m_vstreamDecoder->ConvertFrame( m_frame, m_outFrame );

                char * data = new char[ m_frameSize ];
	            memcpy( data, ( char * )m_outFrame->data[ 0 ], m_frameSize );

	            VideoMediaData mediaData;
	            mediaData.frameIdx = m_vstreamDecoder->GetCurrentFrameId();
	            mediaData.frameData = MemoryChunk::Create( data, SizeType( m_frameSize ) );
	            m_bufferQueue.Push( mediaData );

                if( m_outQueue.Size() == 0 )
                {
                    m_outQueue.Push( mediaData );
                }

		        success = true;
            }

	        av_packet_unref( packet );
	    }
    }

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
UInt32					FFmpegVideoDecoder::GetMaxBufferSize		() const
{
	return MAX_QUEUE_SIZE;
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
        av_packet_unref( packet );
    }
}

// *********************************
//
void					FFmpegVideoDecoder::FlushBuffers			() 
{
	m_outQueue.Clear();
	m_bufferQueue.Clear();
}

// *********************************
//
void					FFmpegVideoDecoder::Reset					() 
{
	std::lock_guard< std::mutex > lock( m_mutex );
	m_demuxer->Reset();
	m_vstreamDecoder->Reset();
	m_outQueue.Clear();
	m_bufferQueue.Clear();
}

// *********************************
//
bool					FFmpegVideoDecoder::IsEOF					() const
{
	std::lock_guard< std::mutex > lock( m_mutex );
    return m_demuxer->IsEOF();
}

// *********************************
//
bool					FFmpegVideoDecoder::IsFinished				() const
{
    return IsEOF() && m_outQueue.IsEmpty() && m_bufferQueue.IsEmpty();
}

} //bv