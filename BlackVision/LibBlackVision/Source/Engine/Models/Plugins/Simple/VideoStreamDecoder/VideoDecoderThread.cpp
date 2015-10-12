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
	, m_totalTime( 0 )
{
}

// *******************************
//
VideoDecoderThread::~VideoDecoderThread				()
{
	Stop();
}

// *******************************
//
void				VideoDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopped = true;
	m_cond.notify_one();

	Join();
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
	auto frameDuration = 1000 / m_decoder->GetFrameRate();

	auto t = m_decoder->GetDuration();
	{t;}

	while( !m_stopped && m_totalTime < m_decoder->GetDuration() )
	{
		if ( m_paused )
		{
			std::unique_lock< std::mutex > lock( m_mutex );
			while( m_paused )
			{
				m_cond.wait( lock );
			}
		}

		if( m_decoder->GetNextFrameData() )
		{
			//FIXME
			auto time = m_timer.ElapsedMillis();
			while( time < frameDuration )
			{
				Sleep( frameDuration - time );
				time = m_timer.ElapsedMillis();
			}

			m_totalTime += frameDuration;
			m_timer.Start();
		}
	}
}

} //bv
