#include "stdafx.h"

#include "FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/Demuxer/FFmpegDemuxer.h"

#include "Util/FFmpeg/FFmpegUtils.h"


namespace bv {


// *******************************
//
FFmpegStreamDecoder::FFmpegStreamDecoder                    ( AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize, FFmpegDemuxer * demuxer )
    : m_streamIdx( streamIdx )
    , m_offset( 0 )
    , m_prevPTS( 0 )
	, m_bufferQueue( maxQueueSize )
	, m_outQueue( 30 )
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

	if( m_codecCtx->channel_layout == 0 )
	{
		m_codecCtx->channel_layout = av_get_default_channel_layout( m_codecCtx->channels );
	}

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
void		        FFmpegStreamDecoder::UploadData         ()
{
    AVMediaData data;
    if( m_bufferQueue.TryPop( data ) )
    {
        m_outQueue.TryPush( data );
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
			LOG_MESSAGE( SeverityLevel::error ) << "Packet decoding error: " << FFmpegUtils::AVErrorToString( res );
			throw std::runtime_error( "Packet decoding error" );
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
namespace
{
// *******************************
//
UInt64              GetFrontMediaDataPTS			( QueueConcurrentLimited< AVMediaData > & queue )
{
	AVMediaData data;
	queue.Front( data );
	return data.framePTS;
}

// *********************************
//
bool	PopFromDataQueue( QueueConcurrentLimited< AVMediaData > & queue, UInt64 time, bool block, AVMediaData & data, UInt64 prevPTS, std::atomic< bool > & interruptWait )
{
	auto success = false;

	if( block )
	{
		auto pn = [ =, &interruptWait ] ( const AVMediaData & avm )
		{
			auto val = ( prevPTS <= avm.framePTS && avm.framePTS <= time ) && !interruptWait;

			return val;
		};

		success = queue.WaitAndPopUntil( data, pn );

		// If the first frame doesn't satisfy the predicate WaitAndPopUntil returns empty frame.
		// We have to upadate success flag accordingly.
		success = data.frameData ? success : false;
	}
	else
	{
		// find the closest frame to given time
		while( !queue.IsEmpty()
			   && ( prevPTS <= GetFrontMediaDataPTS( queue ) )
			   && ( GetFrontMediaDataPTS( queue ) <= time ) )
		{
			success = queue.TryPop( data );
		}
	}

	return success;
}

} // anonymous

// *********************************
//
bool				FFmpegStreamDecoder::NextDataReady      ( UInt64 time, bool block )
{
	time += GetOffset();

	if( time <= m_duration )
	{
		AVMediaData data;

		// Pop from data queue frame closes to a given timestamp.
		if( PopFromDataQueue( m_bufferQueue, time, block, data, m_prevPTS, m_interruptWait ) )
		{
			//LOG_MESSAGE( SeverityLevel::debug )
			//	<< "FFmpegStreamDecoderThread pushing frame "
			//	<< data.framePTS
			//	<< " to queue "
			//	<< m_streamIdx
			//	<< " size "
			//	<< m_outQueue.Size();
			
			{	// Removing to old frames from the out queue. (older than 150 miliseconds are removed)
				AVMediaData oldData;

                auto s = m_outQueue.Size();

				m_outQueue.TryPopUntil( oldData, [ = ] ( const AVMediaData & avm )
				{
					return avm.framePTS + 100 < time;
				} );

                if( s != m_outQueue.Size() )
                {
                    s = s;
                }
			}

			// Push data to the out queue
			if( !m_outQueue.TryPush( data ) )
			{
				// If still cannot push frame clear the queue.
				m_outQueue.Clear();
				m_outQueue.TryPush( data );
			}

			// Update previous frame presentation timestamp. (pts)
			m_prevPTS = data.framePTS;	

			return true;
		}
    }
    else
    {
        m_prevPTS = 0;
    }

    return false;
}

// *******************************
// Returns last played frame presentation time stamp
UInt64              FFmpegStreamDecoder::GetLastPlayedFramePTS          () const
{
    return m_prevPTS - GetOffset();
}

// *******************************
//
UInt64              FFmpegStreamDecoder::GetCurrentPTS					()
{
	return GetFrontMediaDataPTS( m_bufferQueue );
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
