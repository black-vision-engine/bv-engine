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
	m_stopped = false;
    m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopped = true;
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
	std::cout << "Decoder thread stating " << std::this_thread::get_id() << std::endl;
    while( m_running )
    {
		std::unique_lock< std::mutex > lock( m_mutex );
		m_cond.wait( lock, [ = ] { return m_stopped == false; } );
		lock.unlock();

		if( m_videoStreamDecoder && m_audioStreamDecoder )
		{
			auto streamIdx = m_demuxer->GetNextPacketTypeToDecode();

			std::cout << "Decoding steam  " << streamIdx << std::endl;

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