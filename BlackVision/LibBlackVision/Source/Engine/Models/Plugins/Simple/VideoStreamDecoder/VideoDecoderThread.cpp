#include "stdafx.h"

#include "VideoDecoderThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>

namespace bv
{

// *******************************
//
VideoDecoderThread::VideoDecoderThread				( IVideoDecoder * decoder )
	: m_decoder( decoder )
	, m_paused( false )
	, m_stopped( true )
    , m_cancelled( false )
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
		m_cancelled = true;
	}
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Play	    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_paused = false;
		m_stopped = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Stop		()
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
void				VideoDecoderThread::Pause		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_paused = !m_paused;
	}
	m_cond.notify_one();
}

// *******************************
//
bool				VideoDecoderThread::Paused		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_paused;
}

// *******************************
//
bool				VideoDecoderThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				VideoDecoderThread::Run			()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_paused = false;
		m_stopped = true;
	}

    m_timer.Start();

	//FIXME
	auto frameDuration = 1000.0 / m_decoder->GetFrameRate();
    while( !m_cancelled )
    {
		std::unique_lock< std::mutex > lock( m_mutex );

        m_decoder->DecodeNextFrame();

	    if( !m_stopped )
	    {
            if( m_decoder->IsFinished() )
		    {
			    m_paused = false;
			    m_stopped = true;
		    }

            if( ( UInt64 )frameDuration <= m_timer.ElapsedMillis() )
		    {
			    //std::this_thread::sleep_for( std::chrono::milliseconds( ( UInt64 )frameDuration - m_timer.ElapsedMillis() ) );
                m_decoder->NextFrameDataReady();
			    m_timer.Start();
		    }

		    if ( m_paused )
		    {
			    while( m_paused )
			    {
				    m_cond.wait( lock );
				    m_timer.Start();
			    }
		    }
	    }
    }
}

} //bv