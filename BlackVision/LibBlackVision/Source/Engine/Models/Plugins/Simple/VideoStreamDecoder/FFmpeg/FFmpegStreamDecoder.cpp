#include "stdafx.h"

#include "FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"


namespace bv {


// *******************************
//
FFmpegStreamDecoder::FFmpegStreamDecoder                    ( AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize, FFmpegDemuxer * demuxer )
    : m_streamIdx( streamIdx )
    , m_offset( 0 )
    , m_prevPTS( 0 )
	, m_bufferQueue( maxQueueSize )
	, m_outQueue()
	, m_demuxer( demuxer )
	, m_interruptWait( false )
{
    m_stream = formatCtx->streams[ streamIdx ];

    m_codec = avcodec_find_decoder( m_stream->codecpar->codec_id );
    assert( m_codec != nullptr );

    m_codecCtx = avcodec_alloc_context3( m_codec );
	avcodec_parameters_to_context( m_codecCtx, m_stream->codecpar );

    bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
    assert( !error ); { error; }

    m_duration = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_stream->duration );

    m_frame = av_frame_alloc();
}

// *******************************
//
FFmpegStreamDecoder::~FFmpegStreamDecoder                   ()
{
    avcodec_close( m_codecCtx );
    av_frame_free( &m_frame );
	m_bufferQueue.EnqueueEndMessage();
    m_bufferQueue.Clear();
}

// *******************************
//
UInt64              FFmpegStreamDecoder::GetDuration        () const
{
    return m_duration;
}

// *******************************
//
UInt64              FFmpegStreamDecoder::GetCurrentPTS      ()
{
    AVMediaData data;
    m_bufferQueue.Front( data );
    return data.framePTS;
}

// *******************************
//
void		        FFmpegStreamDecoder::UploadData         ()
{
    AVMediaData data;
    if( m_bufferQueue.TryPop( data ) )
    {
        m_outQueue.Push( data );
    }
}

// *******************************
//
void				FFmpegStreamDecoder::ClearDataQueue		()
{
	m_bufferQueue.Clear();
}

// *******************************
//
void				FFmpegStreamDecoder::ClearOutQueue		()
{
	m_outQueue.Clear();
}


// *******************************
//
bool		        FFmpegStreamDecoder::PopData	        ( AVMediaData & data )
{
    return m_outQueue.TryPop( data );
}

// *******************************
//
bool                FFmpegStreamDecoder::IsDataQueueEmpty   () const
{
    return m_bufferQueue.IsEmpty();
}

// *******************************
//
bool                FFmpegStreamDecoder::IsOutQueueEmpty    () const
{
    return m_outQueue.IsEmpty();
}

// *******************************
//
void				FFmpegStreamDecoder::Reset				()
{
    m_prevPTS = 0;
    m_outQueue.Clear();
    m_bufferQueue.Clear();

    avcodec_send_packet( m_codecCtx, nullptr );
    while( avcodec_receive_frame( m_codecCtx, m_frame ) != AVERROR_EOF );

    avcodec_flush_buffers( m_codecCtx );
}

// *******************************
//
bool			    FFmpegStreamDecoder::ProcessPacket      ( bool block )
{
    auto packet = m_demuxer->GetPacket( m_streamIdx, block );
    if( packet )
    {
        if( DecodePacket( packet->GetAVPacket() ) )
        {
            auto data = ConvertFrame();
            m_bufferQueue.WaitAndPush( data );

            return true;
		}
		else
		{
			return block;
		}
    } 
	else if ( block )
	{
		m_bufferQueue.EnqueueEndMessage();
	} 

	return false;
}

// *******************************
//
bool                FFmpegStreamDecoder::DecodePacket       ( AVPacket * packet )
{
	assert( packet != nullptr );

	if( packet->data != nullptr ) // If not dummy message
	{
		auto res = avcodec_send_packet( m_codecCtx, packet );

		if( res == 0 )
		{
			return ( avcodec_receive_frame( m_codecCtx, m_frame ) == 0 );
		}
		else
		{
			auto err = AVUNERROR( res );
			std::cout << "Packet decoding error: " << std::string( ( char * ) &err, 4 ) << std::endl;
			return false;
		}
	}
	else
	{
		return false;
	}
}

// *********************************
//
void			    FFmpegStreamDecoder::SetOffset	        ( UInt64 offset )
{
    m_offset = offset;
}

// *********************************
//
UInt64			    FFmpegStreamDecoder::GetOffset	        () const
{
    return m_offset;
}

// *********************************
//
Int32			    FFmpegStreamDecoder::GetStreamIdx	    () const
{
    return m_streamIdx;
}

// *******************************
//
Int64               FFmpegStreamDecoder::ConvertTime        ( Float64 time )
{
    auto timeBase = av_q2d( m_stream->time_base );
    return ( Int64 )( time / timeBase );
}

// *********************************
//
void				FFmpegStreamDecoder::SetWaitingInterrupt			()
{
	m_interruptWait = !m_interruptWait;
}

// *********************************
//
void				FFmpegStreamDecoder::EnqueueDummyDataMessage		()
{
	m_bufferQueue.TryPush( AVMediaData() );
}

// *********************************
//
bool				FFmpegStreamDecoder::NextDataReady      ( UInt64 time, bool block )
{
    auto success = false;

	if( time <= m_duration )
	{
		AVMediaData data;

		if( block )
		{
			auto offset = GetOffset();

			std::atomic< bool > & flag = m_interruptWait;

			auto pn = [=, &flag ] ( const AVMediaData & avm )
			{
				return ( m_prevPTS <= avm.framePTS && avm.framePTS <= time + offset ) && !flag;
			};

			success = m_bufferQueue.WaitAndPopUntil( data, pn ); // Tutaj dodaæ flagê która przerywa czekanie na nastêpnym dodanym elemencie do kolejki.

			success = data.frameData ? success : false;
		}
		else
		{
			// find the closest frame to given time
			while( !IsDataQueueEmpty()
					&& ( m_prevPTS <= GetCurrentPTS() )
					&& ( GetCurrentPTS() <= time + GetOffset() ) )
			{
				success = m_bufferQueue.TryPop( data );
			}
		}

		if( success )
		{
			m_outQueue.Push( data );
			m_prevPTS = data.framePTS;
		}
    }
    else
    {
        m_prevPTS = 0;
    }

    return success;
}

// *********************************
//
void				FFmpegStreamDecoder::FinishQueue					()
{
	m_bufferQueue.EnqueueEndMessage();
}

// *********************************
//
bool				FFmpegStreamDecoder::IsFinished						() const
{
	bool finished = true;
	finished &= m_demuxer->IsEOF();

	finished &= m_demuxer->IsPacketQueueEmpty( GetStreamIdx() );
	finished &= IsDataQueueEmpty();
	finished &= IsOutQueueEmpty();

	return finished;
}

} //bv
