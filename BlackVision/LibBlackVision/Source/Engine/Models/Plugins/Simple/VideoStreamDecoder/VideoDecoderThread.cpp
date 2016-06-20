#include "stdafx.h"

#include "VideoDecoderThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


namespace bv {


// *******************************
//
VideoDecoderThread::VideoDecoderThread				( IAVDecoder * decoder )
	: m_decoder( decoder )
	, m_stopped( false )
    , m_running( false )
{
}

// *******************************
//
VideoDecoderThread::~VideoDecoderThread				()
{
}

// *******************************
//
void				VideoDecoderThread::Kill	    ()
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
void				VideoDecoderThread::Play	    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = false;
        m_timer.Start();
	}
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
	}
	m_cond.notify_one();
}

// *******************************
//
bool				VideoDecoderThread::Stopped		    () const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				VideoDecoderThread::Run			    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
		m_running = true;
	}

	//FIXME
	auto frameDuration = 1000.0 / m_decoder->GetFrameRate();
    while( true )
    {
		std::unique_lock< std::mutex > lock( m_mutex );

        if( !m_running )
        {
            break;
        }

        m_decoder->NextAudioDataReady();

        if( ( UInt64 )frameDuration <= m_timer.ElapsedMillis() )
		{
			//std::this_thread::sleep_for( std::chrono::milliseconds( ( UInt64 )frameDuration - m_timer.ElapsedMillis() ) );
            m_decoder->NextVideoDataReady();

			m_timer.Start();
		}

        if( m_decoder->IsFinished() )
		{
			m_stopped = true;
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