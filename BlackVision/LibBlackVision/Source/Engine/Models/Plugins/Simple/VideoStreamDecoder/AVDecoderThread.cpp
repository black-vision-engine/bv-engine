#include "stdafx.h"

#include "AVDecoderThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


#include "FFmpeg/FFmpegStreamDecoder.h"

#include "Memory/MemoryLeaks.h"



namespace bv {


// *******************************
//
AVDecoderThread::AVDecoderThread				( FFmpegStreamDecoder * streamDecoder )
	: m_streamDecoder( streamDecoder )
    , m_paused( false )
	, m_stopped( false )
    , m_running( false )
{
}

// *******************************
//
AVDecoderThread::~AVDecoderThread				()
{
}

// *******************************
//
void				AVDecoderThread::Kill	    ()
{
    {
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = false;
		m_running = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Play	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    if( !m_paused )
    {
        m_timer.Start();
    }

	m_stopThread = false;
	m_pauseThread = false;
	m_stopped = false;
    m_paused = false;
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Restart	()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_paused = false;
	m_stopThread = false;
	m_pauseThread = false;
	m_stopped = false;
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    m_paused = false;
	m_stopThread = true;
	m_cond.notify_one();
}

// *******************************
//
bool				AVDecoderThread::Pause		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_pauseThread = true;
	m_cond.notify_one();
    return m_paused;
}

// *******************************
//
bool				AVDecoderThread::Paused		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_paused;
}

// *******************************
//
bool				AVDecoderThread::Stopped		    () const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				AVDecoderThread::Run			    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_paused = false;
		m_stopped = true;
		m_running = true;
	}

    auto duration = m_streamDecoder->GetDuration();
    m_timer.Start();

	std::cout << "AVDecoder thread starting " << std::this_thread::get_id() << std::endl;

    while( m_running )
    {
        auto time = m_timer.ElapsedMillis();
        
		m_streamDecoder->NextDataReady( time, true );

        if( time > duration ) 
        {
			m_timer.Start();
        }

		std::unique_lock< std::mutex > lock( m_mutex );

        if( m_streamDecoder->IsFinished() )
		{
			m_stopped = true;
		}

		if ( m_pauseThread )
		{
			m_paused = true;

			while( m_paused )
			{
                m_timer.Pause();
				m_cond.wait( lock );
                m_timer.UnPause();
			}
		}

		if ( m_stopThread && m_running )
		{
			m_stopped = true;

			while( m_stopped )
			{	
				std::cout << "AVDecoder thread stopped " << std::this_thread::get_id() << std::endl;
				m_cond.wait( lock );
				m_timer.Start();
			}
		}
    }

	std::cout << "AVDecoder thread dying " << std::this_thread::get_id() << std::endl;
}

} //bv