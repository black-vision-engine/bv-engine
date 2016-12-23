#include "stdafx.h"

#include "FFmpegDemuxer.h"

#include <cassert>


namespace bv {


const UInt32         FFmpegDemuxer::SAFE_SEEK_FRAMES = 10;
const UInt32         FFmpegDemuxer::MAX_QUEUE_SIZE = 10;


// *******************************
//FIXME: pass which stream we want - now only video
FFmpegDemuxer::FFmpegDemuxer     ( const std::string & streamPath, UInt32 maxQueueSize )
	: m_streamPath( streamPath )
	, m_formatCtx( nullptr )
	, m_isEOF( false )
    , m_maxQueueSize( maxQueueSize )
{
	av_register_all();

	bool error = false; 

	error = avformat_open_input( &m_formatCtx, streamPath.c_str(), nullptr, nullptr ) != 0;
	assert( !error );

	error = avformat_find_stream_info( m_formatCtx, nullptr ) < 0;
	//assert( !error ); //raw video

	av_dump_format( m_formatCtx, 0, streamPath.c_str(), 0 ); //log
}

// *******************************
//
FFmpegDemuxer::~FFmpegDemuxer    ()
{
	avformat_close_input( &m_formatCtx );
	ClearPacketQueue( false );
}

// *******************************
//
AVFormatContext *	FFmpegDemuxer::GetFormatContext		() const
{
	return m_formatCtx;
}


static int cccc = 0;

static int ccccMax = 0;

static Int64 maxTimeShiftPts = 0;

static Int64 lastVideoFramePts = 0;

// *******************************
//
bool			FFmpegDemuxer::ProcessPacket			()
{

    //FIXME: sync with clock instead of maxQueueSize
    //auto process = false;
    //for( auto & queue : m_packetQueue )
    //{
    //    if( queue.second->Size() < m_maxQueueSize )
    //    {
    //        process = true;
    //    }
    //}

    //if( process )
    {
        auto ffmpegPacket = std::make_shared< FFmpegPacket >();
        auto packet = ffmpegPacket->GetAVPacket();
        auto error = av_read_frame( m_formatCtx, packet );
	    if( error < 0 ) 
        {
			std::lock_guard< std::mutex > lock( m_mutex );
		    assert( error == AVERROR_EOF ); //error reading frame
		    m_isEOF = true;

			for( auto & k : m_packetQueue )
			{
				k.second->EnqueueEndMessage();
			}
            return false;
        }
		else if( error == 0 )
		{
			auto currStream = packet->stream_index;

			if( currStream == 0 )
			{
				cccc = 0;
				if( packet->pts >  lastVideoFramePts )
				{
					auto diff = packet->pts - lastVideoFramePts;
					if( diff > maxTimeShiftPts )
					{
						maxTimeShiftPts = diff;

						std::cout << "maxTimeShiftPts " << maxTimeShiftPts << std::endl;
						std::cout << "maxTimeShiftPts " << packet->pts << "   " << lastVideoFramePts << std::endl;
					}
				}

				lastVideoFramePts = packet->pts;
			}
			else
			{
				cccc++;
			}

			if( ccccMax < cccc )
			{
				ccccMax = cccc;
				std::cout << "ccccMax " << ccccMax << std::endl;
			}

			if( m_packetQueue.count( currStream ) > 0 )
			{
				m_packetQueue.at( currStream )->WaitAndPush( ffmpegPacket );
				return true;
			}
		}
		else 
		{
			std::cout << "Error " << error << std::endl;
		}
    }
    return false;
}

// *******************************
//
FFmpegPacketPtr		FFmpegDemuxer::GetPacket				( Int32 streamIdx, bool block )
{
    FFmpegPacketPtr packet = nullptr;

	assert( m_packetQueue.count( streamIdx ) > 0 );

 //   if( !m_packetQueue.at( streamIdx )->IsEmpty() )
	//{
	if( block )
	{
		m_packetQueue.at( streamIdx )->WaitAndPop( packet );
	}
	else
	{
		m_packetQueue.at( streamIdx )->TryPop( packet );
	}
//	}

	return packet;
}

// *******************************
//
void				FFmpegDemuxer::Seek					( Int64 timestamp, Int32 streamIdx )
{
    auto initTs = timestamp - SAFE_SEEK_FRAMES;
    if( initTs < 0 )
    {
        initTs = 0;
    }

	std::lock_guard< std::mutex > lock( m_mutex );

	av_seek_frame( m_formatCtx, streamIdx, initTs, AVSEEK_FLAG_BACKWARD );
	
	m_isEOF = false; 
}

// *******************************
//
void				FFmpegDemuxer::Reset				()
{
    ClearPacketQueue( false );
	Seek( 0 );
}

static int qS [] = { 10, 10000 };

// *******************************
//
Int32				FFmpegDemuxer::GetStreamIndex	( AVMediaType type, UInt32 idx )
{
	auto streamIdx = FindStreamIndex( type, idx );

    if( streamIdx >= 0 )
    {
	    if( m_packetQueue.count( streamIdx ) == 0 )
	    {
            m_packetQueue.insert( std::make_pair( streamIdx, std::make_shared< QueueConcurrentLimited< FFmpegPacketPtr > >( qS[ type ] ) ) );
	    }
    }

	return streamIdx;
}

// *******************************
//
void			    FFmpegDemuxer::DisableStream    ( AVMediaType type, UInt32 idx )
{
	auto streamIdx = FindStreamIndex( type, idx );
	if( ( streamIdx >= 0 ) && m_packetQueue.count( streamIdx ) )
	{
        ClearPacketQueue( streamIdx, false );
        m_packetQueue.erase( streamIdx );
	}
}

// *******************************
//
bool				FFmpegDemuxer::IsEOF				() const
{
	std::lock_guard< std::mutex > lock( m_mutex );
	return m_isEOF;
}

// *********************************
//
bool			    FFmpegDemuxer::IsPacketQueueEmpty	( Int32 streamIdx ) const
{
    if( m_packetQueue.count( streamIdx ) )
    {
        return m_packetQueue.at( streamIdx )->IsEmpty();
    }
    return true;
}

// *******************************
//
void				FFmpegDemuxer::ClearPacketQueue		( bool removingDemuxer )
{
	for( auto it = m_packetQueue.begin(); it != m_packetQueue.end(); ++it )
	{
        ClearPacketQueue( it->first, removingDemuxer );
	}
}

// *******************************
//
void				FFmpegDemuxer::ClearPacketQueue		( Int32 streamIdx, bool removingDemuxer )
{
    auto & queue = m_packetQueue[ streamIdx ];

	queue->Clear();

	if( removingDemuxer )
	{
		queue->EnqueueEndMessage();
	}

    //while( !queue->IsEmpty() )
    //{
    //    FFmpegPacketPtr packet = nullptr;
    //    queue->TryPop( packet );
    //}
    //queue->Clear();
}

// *******************************
//
Int32				FFmpegDemuxer::FindStreamIndex		( AVMediaType type, UInt32 idx ) const
{
	UInt32 counter = 0;
	Int32 last = -1;
	for ( unsigned int i = 0; i < m_formatCtx->nb_streams; ++i )
	{
        if ( m_formatCtx->streams[i]->codecpar->codec_type == type )
		{
			last = ( Int32 )i;
			if( counter == idx )
			{
				break;
			}
			counter++;
		}
	}
	return last;
}

} //bv