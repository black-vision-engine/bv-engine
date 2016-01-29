#include "stdafx.h"

#include "FFmpegDemuxer.h"

#include <cassert>

namespace bv
{

// *******************************
//FIXME: pass which stream we want - now only video
FFmpegDemuxer::FFmpegDemuxer     ( const std::string & streamPath )
	: m_streamPath( streamPath )
	, m_formatCtx( nullptr )
	, m_isEOF( false )
{
	av_register_all();

	bool error = false; 

	error = avformat_open_input( &m_formatCtx, streamPath.c_str(), nullptr, nullptr ) != 0;
	assert( !error );

	error = avformat_find_stream_info( m_formatCtx, nullptr ) < 0;
	//assert( !error ); //raw video

	av_dump_format( m_formatCtx, 0, streamPath.c_str(), 0 );
}

// *******************************
//
FFmpegDemuxer::~FFmpegDemuxer    ()
{
	avformat_close_input( &m_formatCtx );
	ClearPacketQueue();
}

// *******************************
//
AVFormatContext *	FFmpegDemuxer::GetFormatContext		() const
{
	return m_formatCtx;
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
				assert( error == AVERROR_EOF ); //error reading frame
				
				m_isEOF = true;
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
					delete packet;
                }
            }
			else
			{
                av_dup_packet( packet );
            }
		}
	}

	return packet;
}

// *******************************
//
void				FFmpegDemuxer::Seek					( Int64 timestamp, Int32 streamIdx )
{
	av_seek_frame( m_formatCtx, streamIdx, timestamp, AVSEEK_FLAG_ANY );
	ClearPacketQueue();
}

// *******************************
//
void				FFmpegDemuxer::Reset				()
{
	Seek( 0 );
	m_isEOF = false; 
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
bool				FFmpegDemuxer::IsEOF				() const
{
	return m_isEOF;
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