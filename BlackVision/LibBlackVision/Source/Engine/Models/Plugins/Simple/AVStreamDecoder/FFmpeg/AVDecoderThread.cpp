#include "stdafx.h"

#include "AVDecoderThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


#include "Decoders/FFmpegStreamDecoder.h"

#include "Memory/MemoryLeaks.h"



namespace bv {


// *******************************
//
AVDecoderThread::AVDecoderThread				( FFmpegStreamDecoder * streamDecoder, const SimpleTimer & timer )
	: m_streamDecoder( streamDecoder )
    , m_paused( false )
	, m_stopped( false )
    , m_running( false )
	, m_timer( timer )
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
	std::cout << "KILLING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
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

	std::cout << "START PLAYING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;

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
	if( m_stopped || m_paused || m_stopThread || m_pauseThread )
	{
		std::cout << "RESTARTING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;

		m_paused = false;
		m_stopThread = false;
		m_pauseThread = false;
		m_stopped = false;
		m_cond.notify_one();
	}
}

// *******************************
//
void				AVDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	if( !m_stopThread )
	{
		std::cout << "STOPPING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
		m_pauseThread = false;
		m_paused = false;
		m_stopThread = true;
		m_cond.notify_one();
	}
}

// *******************************
//
void				AVDecoderThread::Pause		()	
{
	std::unique_lock< std::mutex > lock( m_mutex );
	if( !m_pauseThread )
	{
		std::cout << "PAUSING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
		m_pauseThread = true;
		m_cond.notify_one();
	}
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
		m_stopThread = true;
		m_running = true;
	}

    auto duration = m_streamDecoder->GetDuration();

	std::cout << "STARTING AVDecoder thread " << std::this_thread::get_id() << std::endl;

    while( m_running )
    {
        {
            std::unique_lock< std::mutex > lock( m_mutex );
            if( m_stopThread && m_running )
            {
                m_stopped = true;

                while( m_stopped )
                {
                    std::cout << "STOPPED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
                    m_cond.wait( lock );
                    std::cout << "STARTED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
                }
            }
        }

        auto time = m_timer.ElapsedMillis();
        
		m_streamDecoder->NextDataReady( time, true );

		std::unique_lock< std::mutex > lock( m_mutex );
		
		if( time + m_streamDecoder->GetOffset() > duration || m_streamDecoder->IsFinished() )
		{
			std::cout << "STOPPING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
			m_stopThread = true;
		}

		if ( m_pauseThread )
		{
			m_paused = true;

			while( m_paused )
			{
				std::cout << "PAUSED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
				m_cond.wait( lock );
				std::cout << "UNPAUSED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id() << std::endl;
			}
		}
    }

	std::cout << "DYING AVDecoder thread " << std::this_thread::get_id() << std::endl;
}

} //bv