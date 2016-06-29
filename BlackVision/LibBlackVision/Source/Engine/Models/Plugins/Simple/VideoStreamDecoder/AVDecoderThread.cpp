#include "stdafx.h"

#include "AVDecoderThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


namespace bv {


// *******************************
//
AVDecoderThread::AVDecoderThread				( IAVDecoder * decoder )
	: m_decoder( decoder )
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
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_paused = false;
		m_stopped = false;
        m_timer.Start();
	}
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_paused = false;
		m_stopped = true;
	}
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Pause		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_paused = !m_paused;
	}
	m_cond.notify_one();
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

    auto duration = m_decoder->GetDuration();
    m_timer.Start();

    while( true )
    {
		std::unique_lock< std::mutex > lock( m_mutex );

        if( !m_running )
        {
            break;
        }

        auto time = m_timer.ElapsedMillis();

        m_decoder->NextVideoDataReady( time );
        m_decoder->NextAudioDataReady( time );

        if( time > duration ) 
        {
			m_timer.Start();
        }

        if( m_decoder->IsFinished() )
		{
			m_stopped = true;
		}

		if ( m_paused )
		{
			while( m_paused )
			{
				m_cond.wait( lock );
                m_timer.Pause();
			}
		}

		if ( m_stopped )
		{
			while( m_stopped )
			{
				m_cond.wait( lock );
				m_timer.Start();
			}
		}
    }
}

} //bv