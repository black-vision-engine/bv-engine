#include "stdafx.h"

#include "FFmpegDemuxer.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegUtils.h"

#include "UseLoggerLibBlackVision.h"

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
	, m_lastReadFrameIdx( -1 )
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
Int32				FFmpegDemuxer::UpdateMaxFrameNumInSeq	( Int32 streamIdx )
{
	if( m_lastReadFrameIdx == streamIdx )
	{
		if( m_maxNumOfFramesInSeq.count( streamIdx ) == 0 )
		{
			m_maxNumOfFramesInSeq[ streamIdx ] = 1;
			m_currentFrameCount = 1;
		}
		else
		{
			m_currentFrameCount++;
			if( m_maxNumOfFramesInSeq[ streamIdx ] < m_currentFrameCount )
			{
				m_maxNumOfFramesInSeq[ streamIdx ] = m_currentFrameCount;
			}
		}

		return m_maxNumOfFramesInSeq[ streamIdx ];
	}
	else
	{
		m_lastReadFrameIdx = streamIdx;
		m_currentFrameCount = 0;
		return UpdateMaxFrameNumInSeq( streamIdx );
	}
}

// *******************************
//
AVFormatContext *	FFmpegDemuxer::GetFormatContext		() const
{
	return m_formatCtx;
}

// *******************************
//
bool				FFmpegDemuxer::IsAnyQueueFull		() const
{
	for( auto & q : m_packetQueue )
	{
		if( q.second->IsFull() )
		{
			return true;
		}
	}

	return false;
}

// *******************************
//
bool				FFmpegDemuxer::IsQueueFull			( Int32 streamIdx ) const
{
	if( m_packetQueue.count( streamIdx ) > 0 )
	{
		return m_packetQueue.at( streamIdx )->IsEmpty();
	}	

	return false;
}

// *******************************
//
bool			FFmpegDemuxer::ProcessPacket			()
{
    auto ffmpegPacket = std::make_shared< FFmpegPacket >();
    auto packet = ffmpegPacket->GetAVPacket();
    auto error = av_read_frame( m_formatCtx, packet );
	if( error < 0 ) 
    {
		std::lock_guard< std::mutex > lock( m_mutex );
		assert( error == AVERROR_EOF ); //error reading frame
		m_isEOF = true;

        return false;
    }
	else if( error == 0 )
	{
		auto currStream = packet->stream_index;

		if( m_packetQueue.count( currStream ) > 0 )
		{
			//std::cout 
			//	<< "Demuxer's pushing pts " 
			//	<<  ffmpegPacket->GetAVPacket()->pts 
			//	<< " dts "
			//	<< ffmpegPacket->GetAVPacket()->dts
			//	<< " to queue " 
			//	<< currStream 
			//	<< " size " 
			//	<< m_packetQueue.at( currStream )->Size() 
			//	<< " current size "
			//	<< numTypeFrame[ current ]
			//	<< std::endl;
			
			UpdateMaxFrameNumInSeq( currStream );

			m_packetQueue.at( currStream )->WaitAndPush( ffmpegPacket );
			return true;
		}
	}
	else 
	{
        LOG_MESSAGE( SeverityLevel::error ) << "Error " << FFmpegUtils::AVErrorToString( error );
	}
    
	return false;
}

// *******************************
//
FFmpegPacketPtr		FFmpegDemuxer::GetPacket				( Int32 streamIdx, bool block )
{
    FFmpegPacketPtr packet = nullptr;

	assert( m_packetQueue.count( streamIdx ) > 0 );

	if( block )
	{
		m_packetQueue.at( streamIdx )->WaitAndPop( packet );
	}
	else
	{
		m_packetQueue.at( streamIdx )->TryPop( packet );
	}

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

	//auto f = File::Open( "test.txt", File::OpenMode::FOMWriteAppend );

	//std::stringstream out;

	//out << "Demuxer seek to ts: " << timestamp << std::endl;

	//f.Write( out.str() );

	//f.Close();

	av_seek_frame( m_formatCtx, streamIdx, initTs, AVSEEK_FLAG_BACKWARD );
	
	if( initTs != 0 )
	{
		initTs = initTs;
	}

	m_isEOF = false; 

	m_maxNumOfFramesInSeq.clear();
	m_lastReadFrameIdx = -1;
	m_currentFrameCount = 0;
}

// *******************************
//
void				FFmpegDemuxer::Reset				()
{
    ClearPacketQueue( false );
	Seek( 0 );
}

static int qS [] = { 1000, 1000 };

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
void				FFmpegDemuxer::EnqueueDummyMessage	( Int32 streamIdx )
{
	if( m_packetQueue.count( streamIdx ) )
	{
		m_packetQueue.at( streamIdx )->TryPush( std::make_shared< FFmpegPacket >() );
	}
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

// *******************************
//
AVMediaType			FFmpegDemuxer::GetNextPacketTypeToDecode() const
{
	if( m_packetQueue.size() == 2 )
	{
		FFmpegPacketPtr packet0, packet1;
		m_packetQueue.at( 0 )->Front( packet0 );
		m_packetQueue.at( 1 )->Front( packet1 );

		if( packet1 && packet0 )
		{
			return packet0->GetAVPacket()->pts > packet1->GetAVPacket()->pts ? AVMediaType( 1 ) : AVMediaType( 0 );
		}
		else
		{
			return packet1 ? AVMediaType( 1 ) : AVMediaType( 0 );
		}
	}
	else
	{
		return AVMediaType( 0 );
	}
}

} //bv