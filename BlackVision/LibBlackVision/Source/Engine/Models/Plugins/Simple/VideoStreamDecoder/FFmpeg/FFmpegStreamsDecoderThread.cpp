#include "stdafx.h"

#include "FFmpegStreamsDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAVDecoder.h"
#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


namespace bv {


// *******************************
//
FFmpegStreamsDecoderThread::FFmpegStreamsDecoderThread          ( FFmpegStreamDecoder * videoStreamDecoder, FFmpegStreamDecoder * audioStreamDecoder, FFmpegDemuxer * demuxer )
	: m_videoStreamDecoder( videoStreamDecoder )
    , m_audioStreamDecoder( audioStreamDecoder )
    , m_demuxer( demuxer )
	, m_stopped( false )
	, m_stopThread( false )
    , m_running( true )
{
}

// *******************************
//
FFmpegStreamsDecoderThread::~FFmpegStreamsDecoderThread         ()
{
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Kill	        ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_running = false;
    m_stopped = false;

	if( m_audioStreamDecoder )
		m_audioStreamDecoder->Reset();

	if( m_videoStreamDecoder )
		m_videoStreamDecoder->Reset();
	
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Restart	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopThread = false;
	m_stopped = false;
    m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopThread = true;
	m_cond.notify_one();
}

// *******************************
//
bool				FFmpegStreamsDecoderThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Run			()
{
	std::cout << "Decoder thread starting " << std::this_thread::get_id() << std::endl;
    while( true )
    {
		{
			std::unique_lock< std::mutex > lock( m_mutex );

			if( m_stopThread )
			{
				m_stopped = true;
				m_stopThread = false;
			}

			m_cond.wait( lock, [ = ] { return m_stopped == false; } );
		}

		if( !m_running )
		{
			break;
		}

		if( m_videoStreamDecoder && m_audioStreamDecoder )
		{
			auto streamIdx = m_demuxer->GetNextPacketTypeToDecode();

			if( m_videoStreamDecoder->GetStreamIdx() == streamIdx )
			{
				m_videoStreamDecoder->ProcessPacket( m_demuxer, true );
			}
			else
			{
				m_audioStreamDecoder->ProcessPacket( m_demuxer, true );
			}
		}
		else
		{
			if( m_videoStreamDecoder )
			{
				m_videoStreamDecoder->ProcessPacket( m_demuxer, true );
			}

			if( m_audioStreamDecoder )
			{
				m_audioStreamDecoder->ProcessPacket( m_demuxer, true );
			}
		}
    }

	std::cout << "Decoder thread dying " << std::this_thread::get_id() << std::endl;
}

} //bv