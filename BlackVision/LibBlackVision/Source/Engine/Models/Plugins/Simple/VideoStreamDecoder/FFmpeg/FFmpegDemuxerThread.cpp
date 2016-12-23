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

	std::cout << "Killing demuxer thread " << std::this_thread::get_id() << std::endl;

    m_cond.notify_one();
}

// *******************************
//
void				FFmpegDemuxerThread::Restart	()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_stopped = false;
	std::cout << "Demuxer thread restatring " << std::this_thread::get_id() << std::endl;
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegDemuxerThread::Stop		()
{
    std::unique_lock< std::mutex > lock( m_mutex );
    m_stopped = true;
	std::cout << "Demuxer thread stopping " << std::this_thread::get_id() << std::endl;
	m_cond.notify_one();
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
	std::cout << "Demuxer thread starting " << std::this_thread::get_id() << std::endl;

    while( true )
    {        
		std::unique_lock< std::mutex > lock( m_mutex );
		
		if( m_stopped )
		{
			std::cout << "Demuxer thread stopped " << std::this_thread::get_id() << std::endl;
			std::cout << "Queue 0 empty " << m_demuxer->IsPacketQueueEmpty( 0 ) << std::endl;
			std::cout << "Queue 1 empty " << m_demuxer->IsPacketQueueEmpty( 1 ) << std::endl;
		}
		
		m_cond.wait( lock, [ = ] { return m_stopped == false; } );
		lock.unlock();

		if( !m_running )
		{
			break;
		}

		// std::cout << "Demuxer thread process packet. " << std::this_thread::get_id() << std::endl;
        m_demuxer->ProcessPacket();

        if( m_demuxer->IsEOF() )
        {
			std::unique_lock< std::mutex > lock( m_mutex );
            m_stopped = true;
        }
    }

	std::cout << "Demuxer thread dying " << std::this_thread::get_id() << std::endl;
}

} //bv