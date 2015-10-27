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
	
	auto videoIdx = FindStreamIndex( AVMEDIA_TYPE_VIDEO );
	m_streams[ AVMEDIA_TYPE_VIDEO ].push_back( videoIdx );
	m_packetQueue[ videoIdx ] = PacketQueue();
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
                if ( m_packetQueue.count( packet->stream_index ) <= 0 )
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
	int flags = AVSEEK_FLAG_BACKWARD;
	av_seek_frame( m_formatCtx, -1, ( long long )( time * AV_TIME_BASE ), flags );
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


// *******************************
//
bool				FFmpegDemuxer::IsOpened				() const
{
	return m_isOpened;
}

// *******************************
//
bool				FFmpegDemuxer::HasVideoStream		() const
{
	return ( m_streams.count( AVMEDIA_TYPE_VIDEO ) > 0 );
}

// *******************************
//
Int32				FFmpegDemuxer::GetVideoStreamIndex	( UInt32 idx ) const
{
	assert( m_streams.count( AVMEDIA_TYPE_VIDEO ) > 0 );
	assert( m_streams.at( AVMEDIA_TYPE_VIDEO ).size() >= idx );
	return m_streams.at( AVMEDIA_TYPE_VIDEO ).at( idx );
}

// *******************************
//
AVFormatContext *	FFmpegDemuxer::GetFormatContext		() const
{
	return m_formatCtx;
}

} //bv
