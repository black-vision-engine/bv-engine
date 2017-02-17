#include "stdafxCore.h"

#include "StoppableThread.h"

namespace bv {


//**************************************
//
StoppableThread::StoppableThread								()
	: m_running( true )
	, m_stopped( false )
	, m_stop( false )
{}

//**************************************
//
void				StoppableThread::Kill						()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	if( m_stop )
		m_stop = false;

	if( m_stopped )
		m_stopped = false;

	if( m_running )
		m_running = false;

	m_waitResumed.notify_all();
}

//**************************************
//
void				StoppableThread::Stop						()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	if( !m_stopped )
		m_stop = true;
}

//**************************************
//
void				StoppableThread::WaitUntilStopped			()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_waitStopped.wait( lock, [ = ]{ return m_stopped == true; } );
}

//**************************************
//
void				StoppableThread::Resume						()
{
	std::unique_lock< std::mutex > lock( m_mutex );

	if( m_stopped )
	{
		m_stop = false;
		m_stopped = false;
		m_waitResumed.notify_one();
	}
}

//**************************************
//
void				StoppableThread::Run				    	()
{
	while( m_running )
	{
		{
			if( m_stop )
			{
				std::unique_lock< std::mutex > lock( m_mutex );

				m_stopped = true;
				m_waitStopped.notify_all();
				m_waitResumed.wait( lock, [ = ] { return m_stopped == false; } );
			}
		}

		Process();
	}
}

} // bv