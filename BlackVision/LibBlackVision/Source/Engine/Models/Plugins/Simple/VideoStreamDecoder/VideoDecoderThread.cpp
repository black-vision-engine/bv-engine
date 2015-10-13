#include "VideoDecoderThread.h"

#include "Tools/HRTimer.h"

namespace bv
{

// *******************************
//
VideoDecoderThread::VideoDecoderThread				( IVideoDecoderPtr decoder )
	: m_decoder( decoder )
	, m_paused( false )
	, m_stopped( false )
{
}

// *******************************
//
VideoDecoderThread::~VideoDecoderThread				()
{
	Stop();
	Join();
}

// *******************************
//
void				VideoDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopped = true;
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Pause		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_paused = true;
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Resume		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_paused = false;
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Run			()
{
    m_timer.Start();

	auto frameDuration = 1000.0 / m_decoder->GetFrameRate();
	auto frames = m_decoder->GetDuration();
	while( !m_stopped && frames > 0 )
	{
		if ( m_paused )
		{
			std::unique_lock< std::mutex > lock( m_mutex );
			while( m_paused )
			{
				m_cond.wait( lock );
			}
		}

		if( m_decoder->NextFrameDataReady() )
		{
			//
			auto time = m_timer.ElapsedMillis();
			while( time < frameDuration )
			{
				Sleep( ( UInt32 )frameDuration - time );
				time = m_timer.ElapsedMillis();
			}

			m_timer.Start();
		}
		
		frames--;
	}

	Stop();
}

} //bv
