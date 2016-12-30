#include "stdafx.h"

#include "FFmpegStreamsDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAVDecoder.h"
#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


namespace bv {


// *******************************
//
FFmpegStreamsDecoderThread::FFmpegStreamsDecoderThread          ( FFmpegStreamDecoder * streamDecoder )
	: m_streamDecoder( streamDecoder )
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

	std::cout << "KILLING Decoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;

	if( m_streamDecoder )
		m_streamDecoder->Reset();
	
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Restart	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	if( m_stopped || m_stopThread )
	{
		std::cout << "RESTARTING Decoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
		m_stopThread = false;
		m_stopped = false;
		m_cond.notify_one();
	}
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	std::cout << "STOPPING Decoder thread strema id: "<< m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
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
	std::cout << "STARTING Decoder thread " << std::this_thread::get_id() << std::endl;
    while( true )
    {
		{
			std::unique_lock< std::mutex > lock( m_mutex );

			if( m_stopThread )
			{
				m_stopped = true;
				m_stopThread = false;
				std::cout << "STOPPED Decoder thread strema id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
				m_cond.wait( lock, [ = ] { return m_stopped == false; } );
				std::cout << "STARTED Decoder thread strema id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
			}

			if( !m_running )
			{
				break;
			}
		}

		if( m_streamDecoder )
		{
			m_streamDecoder->ProcessPacket( true );
		}
    }

	std::cout << "DYING Decoder thread " << std::this_thread::get_id() << std::endl;
}

} //bv