#include "stdafx.h"

#include "ProcessingThread.h"

#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>

#include "Memory/MemoryLeaks.h"


namespace bv { namespace videocards {


// *******************************
//
ProcessingThread::ProcessingThread				( VideoCardManager * videoCardManager )
	: m_running( false )
    , m_videoCardManager( videoCardManager )
{
}

// *******************************
//
ProcessingThread::~ProcessingThread				()
{
}

// *******************************
//
void				ProcessingThread::Kill	    ()
{
    {
		std::unique_lock< std::mutex > lock( m_mutex );
		m_running = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				ProcessingThread::Run			    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_running = true;
	}

    while( true )
    {
		std::unique_lock< std::mutex > lock( m_mutex );

        if( !m_running )
        {
            break;
        }


    }
}

} //videocards
} //bv