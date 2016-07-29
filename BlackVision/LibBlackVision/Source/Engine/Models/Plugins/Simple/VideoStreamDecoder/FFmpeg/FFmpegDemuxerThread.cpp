#include "stdafx.h"

#include "FFmpegDemuxerThread.h"

#include "Tools/HRTimer.h"

#include <thread>




#include "Memory/MemoryLeaks.h"



namespace bv {

// *******************************
//
FFmpegDemuxerThread::FFmpegDemuxerThread			( FFmpegDemuxer * demuxer )
	: m_demuxer( demuxer )
	, m_stopped( false )
    , m_running( true )
{
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
    {
		std::unique_lock< std::mutex > lock( m_mutex );
		m_running = false;
        m_stopped = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegDemuxerThread::Restart	()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegDemuxerThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
	}
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
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_stopped = true;
		m_running = true;
	}

    while( true )
    {
		std::unique_lock< std::mutex > lock( m_mutex );
        
        if( !m_running )
        {
            break;
        }

        if( m_stopped )
		{
			while( m_stopped )
			{
				m_cond.wait( lock );
			}
		}

        if( !m_demuxer->ProcessPacket() || m_demuxer->IsEOF() )
        {
            m_stopped = true;
        }
    }
}

} //bv