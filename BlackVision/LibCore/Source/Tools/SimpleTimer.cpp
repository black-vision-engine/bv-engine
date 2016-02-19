#include "SimpleTimer.h"

#include "System/Time.h"

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
void  SimpleTimer::Start            ()
{
    m_paused = false;

    m_startPause = 0;
    m_totalPausedTime = 0;

#ifdef QPF_TIMER
	LARGE_INTEGER freq;

	QueryPerformanceFrequency( &freq );
	m_timerFrequency = (unsigned long)freq.QuadPart / 1000;

	m_startMillis = QueryMillis();		// Must be after query frequency.
#else
	m_startMillis = Time::Now();
#endif
}

// *********************************
//
void  SimpleTimer::Pause            ()
{
    if ( !m_paused )
    {
        m_paused = true;
        m_startPause = ( unsigned long )Time::Now();
    }
}

// *********************************
//
void  SimpleTimer::UnPause         ()
{
    if ( m_paused )
    {
        m_paused = false;

		m_totalPausedTime += QueryMillis() - m_startPause;
    }
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
