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
		m_stopped = false;
        m_videoTimer.Start();
        m_audioTimer.Start();
	}
	m_cond.notify_one();
}

// *******************************
//
void				AVDecoderThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
	}
	m_cond.notify_one();
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
		m_stopped = true;
		m_running = true;
	}

	//FIXME
	auto videoFrameDuration = 1000.0 / m_decoder->GetFrameRate();

    auto audioFrameDuration = 1000.0 / m_decoder->GetSampleRate();
    SizeType samples = 0;

    while( true )
    {
		std::unique_lock< std::mutex > lock( m_mutex );

        if( !m_running )
        {
            break;
        }

        //FIXME: just for testing - sync with external clock

        if( ( UInt64 )samples * audioFrameDuration / 1.1f <= m_audioTimer.ElapsedMillis() )
        {
            samples = m_decoder->NextAudioDataReady();

			m_audioTimer.Start();
        }

        if( ( UInt64 )videoFrameDuration <= m_videoTimer.ElapsedMillis() )
		{
			//std::this_thread::sleep_for( std::chrono::milliseconds( ( UInt64 )frameDuration - m_timer.ElapsedMillis() ) );
            m_decoder->NextVideoDataReady();

			m_videoTimer.Start();
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
				m_videoTimer.Start();
				m_audioTimer.Start();
			}
		}
    }
}

} //bv