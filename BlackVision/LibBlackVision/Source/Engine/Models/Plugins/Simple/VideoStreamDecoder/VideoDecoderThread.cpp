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
}

// *******************************
//
void				VideoDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_paused = false;
	m_stopped = true;
	m_cond.notify_one();
}

// *******************************
//
void				VideoDecoderThread::Pause		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_paused = !m_paused;
	m_cond.notify_one();
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
    m_timer.Start();

	//FIXME
	auto frameDuration = 1000.0 / m_decoder->GetFrameRate();
	while( !m_stopped )
	{
		if( m_decoder->IsEOF() )
		{
			Stop();
			break;
		}

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
			auto time = m_timer.ElapsedMillis();
			while( time < frameDuration )
			{
				Sleep( ( UInt32 )frameDuration - time );
				time = m_timer.ElapsedMillis();
			}

			m_timer.Start();
		}
	}
}

} //bv
