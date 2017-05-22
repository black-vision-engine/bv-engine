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
    LOG_MESSAGE( SeverityLevel::debug ) << "KILLING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
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

    LOG_MESSAGE( SeverityLevel::debug ) << "START PLAYING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();

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
        LOG_MESSAGE( SeverityLevel::debug ) << "RESTARTING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();

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
        LOG_MESSAGE( SeverityLevel::debug ) << "STOPPING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
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
        LOG_MESSAGE( SeverityLevel::debug ) << "PAUSING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
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

    LOG_MESSAGE( SeverityLevel::debug ) << "STARTING AVDecoder thread " << std::this_thread::get_id();

    while( m_running )
    {
        {
            std::unique_lock< std::mutex > lock( m_mutex );
            if( m_stopThread && m_running )
            {
                m_stopped = true;

                while( m_stopped )
                {
                    LOG_MESSAGE( SeverityLevel::debug ) << "STOPPED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
                    m_cond.wait( lock );
                    LOG_MESSAGE( SeverityLevel::debug ) << "STARTED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
                }
            }
        }

        auto time = m_timer.ElapsedMillis();
        
		m_streamDecoder->NextDataReady( time, true );

        if( m_streamDecoder->IsAudio() ) 
            while( !m_streamDecoder->IsDataQueueEmpty() && m_streamDecoder->GetCurrentPTS() < time + 50 )
                m_streamDecoder->NextDataReady( m_streamDecoder->GetCurrentPTS(), true );

		std::unique_lock< std::mutex > lock( m_mutex );
		
		if( time + m_streamDecoder->GetOffset() > duration || m_streamDecoder->IsFinished() )
		{
            LOG_MESSAGE( SeverityLevel::debug ) << "STOPPING AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
			m_stopThread = true;
		}

		if ( m_pauseThread )
		{
			m_paused = true;

			while( m_paused )
			{
                LOG_MESSAGE( SeverityLevel::debug ) << "PAUSED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
				m_cond.wait( lock );
                LOG_MESSAGE( SeverityLevel::debug ) << "UNPAUSED AVDecoder thread stream id: " << m_streamDecoder->GetStreamIdx() << " thread id: " << std::this_thread::get_id();
			}
		}
    }

    LOG_MESSAGE( SeverityLevel::debug ) << "DYING AVDecoder thread " << std::this_thread::get_id();
}

} //bv