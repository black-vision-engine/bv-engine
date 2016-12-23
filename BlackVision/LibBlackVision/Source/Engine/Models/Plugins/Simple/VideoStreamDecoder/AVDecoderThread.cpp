#include "stdafx.h"

#include "AVDecoderThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>




#include "Memory/MemoryLeaks.h"



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
	std::unique_lock< std::mutex > lock( m_mutex );
    if( !m_paused )
    {
        m_timer.Start();
    }
	m_stopped = false;
    m_paused = false;
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    m_paused = false;
	m_stopped = true;
	m_cond.notify_one();
}

// *******************************
//
bool				AVDecoderThread::Pause		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_paused = !m_paused;
	m_cond.notify_one();
    return m_paused;
}

// *******************************
//
bool				AVDecoderThread::Paused		() const
{
	return m_paused;
}

// *******************************
//
bool				AVDecoderThread::Stopped		    () const
{
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

	std::cout << "AVDecoder thread stating " << std::this_thread::get_id() << std::endl;


    while( m_running )
    {
        auto time = m_timer.ElapsedMillis();
        
        m_decoder->NextDataReady( time, true );

        if( time > duration ) 
        {
			m_timer.Start();
        }

		std::unique_lock< std::mutex > lock( m_mutex );

        if( m_decoder->IsFinished() )
		{
			m_stopped = true;
		}

		if ( m_paused )
		{
			while( m_paused )
			{
                m_timer.Pause();
				m_cond.wait( lock );
                m_timer.UnPause();
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

		//time+= 1;
    }

	std::cout << "AVDecoder thread dying " << std::this_thread::get_id() << std::endl;
}

} //bv