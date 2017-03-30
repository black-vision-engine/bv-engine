#include "stdafxCore.h"

#include "SimpleTimer.h"

#include "System/Time.h"

#include <future>

#include "win_sock.h"

namespace bv
{

// *********************************
//
SimpleTimer::SimpleTimer            ()
{
    Start();
	timeBeginPeriod( 1 );
}

// *********************************
//
SimpleTimer::~SimpleTimer           ()
{
	timeEndPeriod( 1 );
}

// *********************************
//
void  SimpleTimer::Reset			()
{
	std::unique_lock< std::mutex > lock( m_mutex );

	m_startMillis = QueryMillis();
	m_startPause = m_startMillis;
	m_totalPausedTime = 0;
}

// *********************************
//
void  SimpleTimer::Start            ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    m_paused = false;

	m_startPause = 0;
	m_totalPausedTime = 0;

#ifdef QPF_TIMER
	LARGE_INTEGER freq;

	QueryPerformanceFrequency( &freq );
	m_timerFrequency = ( unsigned long ) freq.QuadPart / 1000;

	m_startMillis = QueryMillis();		// Must be after query frequency.
#else
	m_startMillis = Time::Now();
#endif
}

// *********************************
// Pauses timer when time >= t
void  SimpleTimer::PauseOnAsync    ( UInt64 t )
{
    if( t > ElapsedMillis() )
    {
        std::async( [ = ] ()
        {
            auto elapsed = ElapsedMillis();
            while( elapsed < t && !m_paused )
                elapsed = ElapsedMillis();
            Pause();
        } );
    }
    else
    {
        Pause();
    }
}

// *********************************
//
void  SimpleTimer::Pause            ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    if ( !m_paused )
    {
        m_paused = true;
        m_startPause = QueryMillis();
    }
}

// *********************************
//
void  SimpleTimer::UnPause         ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    if ( m_paused )
    {
        m_paused = false;

		m_totalPausedTime += QueryMillis() - m_startPause;
    }
}

// *********************************
//
unsigned long    SimpleTimer::GetTickCount    ()
{
    return ::GetTickCount();
}

// *********************************
//
unsigned long    SimpleTimer::TimeGetTime     ()
{
    return timeGetTime();
}

// *********************************
//
unsigned long SimpleTimer::QueryMillis() const
{
#ifdef QPF_TIMER
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );

	return unsigned long( currentTime.QuadPart / m_timerFrequency );
#else
	return Time::Now();
#endif
}


} //bv
