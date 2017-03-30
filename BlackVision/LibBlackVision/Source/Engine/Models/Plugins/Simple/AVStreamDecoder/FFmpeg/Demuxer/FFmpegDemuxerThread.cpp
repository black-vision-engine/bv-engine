#include "stdafx.h"

#include "FFmpegDemuxerThread.h"

#include "Tools/HRTimer.h"

#include <thread>


namespace bv {

// *******************************
//
FFmpegDemuxerThread::FFmpegDemuxerThread			( FFmpegDemuxer * demuxer )
	: m_demuxer( demuxer )
	, m_stopped( false )
	, m_stopThread( false )
    , m_running( false )
{
    if( m_demuxer )
    {
        m_running = true;
    }
}

// *******************************
//
FFmpegDemuxerThread::~FFmpegDemuxerThread				()
{
}


// *******************************
//
void				FFmpegDemuxerThread::Kill	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_running = false;
    m_stopped = false;
	m_demuxer->ClearPacketQueue( true );

	LOG_MESSAGE( SeverityLevel::debug ) << "KILLING demuxer thread " << std::this_thread::get_id();

    m_cond.notify_one();
}

// *******************************
//
void				FFmpegDemuxerThread::Restart	()
{
    std::unique_lock< std::mutex > lock( m_mutex );
	if( m_stopThread )
	{
		m_stopThread = false;
		m_stopped = false;
        LOG_MESSAGE( SeverityLevel::debug ) << "RESTARTING Demuxer thread " << std::this_thread::get_id();
		m_cond.notify_one();
	}
}

// *******************************
//
void				FFmpegDemuxerThread::Stop		()
{
    std::unique_lock< std::mutex > lock( m_mutex );

	if( !m_stopThread )
	{
        LOG_MESSAGE( SeverityLevel::debug ) << "STOPPING Demuxer thread " << std::this_thread::get_id();
		m_stopThread = true;
		m_cond.notify_one();
	}

}

// *******************************
//
bool				FFmpegDemuxerThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				FFmpegDemuxerThread::Run			()
{
    LOG_MESSAGE( SeverityLevel::debug ) << "STARTING Demuxer thread " << std::this_thread::get_id();

    while( true )
    {        
		{
			std::unique_lock< std::mutex > lock( m_mutex );
		
			if( m_stopThread )
			{
                LOG_MESSAGE( SeverityLevel::debug ) << "STOPPED Demuxer thread " << std::this_thread::get_id();
                LOG_MESSAGE( SeverityLevel::debug ) << "Queue 0 empty " << m_demuxer->IsPacketQueueEmpty( 0 );
                LOG_MESSAGE( SeverityLevel::debug ) << "Queue 1 empty " << m_demuxer->IsPacketQueueEmpty( 1 );
				m_stopped = true;
				m_cond.wait( lock, [ = ] { return m_stopped == false; } );
                LOG_MESSAGE( SeverityLevel::debug ) << "STARTED Demuxer thread " << std::this_thread::get_id();
			}
		

			if( !m_running )
			{
				break;
			}
		}

		// std::cout << "Demuxer thread process packet. " << std::this_thread::get_id() << std::endl;
        m_demuxer->ProcessPacket();

        if( m_demuxer->IsEOF() )
        {
			std::unique_lock< std::mutex > lock( m_mutex );
			m_stopThread = true;
        }
    }

    LOG_MESSAGE( SeverityLevel::debug ) << "DYING Demuxer thread  " << std::this_thread::get_id();
}

} //bv