#include "FFmpegDemuxer.h"

#include <cassert>

namespace bv
{

// *******************************
//FIXME: pass which stream we want - now only video
FFmpegDemuxer::FFmpegDemuxer     ( const std::string & streamPath )
	: m_formatCtx( nullptr )
	, m_streamPath( streamPath )
	, m_isOpened( false )
	, m_lastPacket( nullptr )
{
	av_register_all();

	bool error = false; 

	error = avformat_open_input( &m_formatCtx, streamPath.c_str(), nullptr, nullptr ) != 0;
	assert( !error );

	error = avformat_find_stream_info( m_formatCtx, nullptr ) < 0;
	assert( !error );

	//FIXME: handle raw
	/*bool error = avformat_find_stream_info( formatCtx, nullptr ) < 0;
	if( error && 
		desc->GetWidth() > 0 && 
		desc->GetHeight() > 0 &&
		desc->GetFrameRate() > 0 )
	{
		m_codecCtx->width = desc->GetWidth();
		m_codecCtx->height = desc->GetHeight();

		AVRational frameRate;
		frameRate.num = desc->GetFrameRate();
		frameRate.den = 1;
		m_codecCtx->framerate = frameRate;

		error = false;
	}
	assert( !error );*/

	m_isOpened = true;

	av_dump_format( m_formatCtx, 0, streamPath.c_str(), 0 );
}

// *******************************
//
FFmpegDemuxer::~FFmpegDemuxer    ()
{
	if ( m_lastPacket )
	{
        av_free_packet( m_lastPacket );
    }

	avformat_close_input( &m_formatCtx );

	ClearPacketQueue();
}


// *******************************
//
bool				FFmpegDemuxer::IsOpened				() const
{
	return m_isOpened;
}

// *******************************
//
AVFormatContext *	FFmpegDemuxer::GetFormatContext		() const
{
	return m_formatCtx;
}

// *******************************
//
UInt32				FFmpegDemuxer::GetDuration			() const
{
	return ( UInt32 )( m_formatCtx->duration / 1000 );
}

// *******************************
//
AVPacket *			FFmpegDemuxer::GetPacket				( Int32 streamIdx )
{
	assert( m_packetQueue.count( streamIdx ) > 0 );

	AVPacket * packet = nullptr;

	if( !m_packetQueue.at( streamIdx ).empty() )
	{
		packet = m_packetQueue.at( streamIdx ).front();
		m_packetQueue.at( streamIdx ).pop_front();
	}
	else 
	{
		Int32 currStream = -1;
		while( currStream != streamIdx )
		{
			packet = new AVPacket();
			auto error = av_read_frame( m_formatCtx, packet );
			if( error < 0 ) {
				assert( error == AVERROR_EOF );	//FIXME: error decoding video
                av_free_packet( packet );
                delete packet;
                return nullptr;
            }

			currStream = packet->stream_index;
			if ( currStream != streamIdx )
			{
                if ( m_packetQueue.count( packet->stream_index ) > 0 )
				{
                    av_dup_packet( packet );
					m_packetQueue[ packet->stream_index ].push_back( packet );
				}
				else
				{
                    av_free_packet( packet );
                }
            }
			else
			{
                av_dup_packet( packet );
				//queue packet?
            }
		}
	}

	return packet;
}

// *******************************
//
void				FFmpegDemuxer::Seek					( Float32 time )
{
	//FIXME
	av_seek_frame( m_formatCtx, -1, ( long long )( time * AV_TIME_BASE ), AVSEEK_FLAG_BACKWARD );
	ClearPacketQueue();
}

// *******************************
//
Int32				FFmpegDemuxer::GetStreamIndex	( AVMediaType type, UInt32 idx )
{
	auto streamIdx = FindStreamIndex( type, idx );
	assert( streamIdx >= 0 );
	
	if( m_packetQueue.count( streamIdx ) == 0 )
	{
		m_packetQueue[ streamIdx ] = PacketQueue();
	}

	return streamIdx;
}

// *******************************
//
void				FFmpegDemuxer::ClearPacketQueue		()
{
	for( auto it = m_packetQueue.begin(); it != m_packetQueue.end(); ++it )
	{
		auto queue = it->second;

		for( auto qit = queue.begin(); qit != queue.end(); ++qit )
		{
			av_free_packet( *qit );
			delete *qit;
		}

		queue.clear();
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
		if ( m_formatCtx->streams[i]->codec->codec_type == type )
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
