#include "SimpleTimer.h"


namespace bv
{

// *********************************
//
SimpleTimer::SimpleTimer            ()
{
    Start();
}

// *********************************
//
SimpleTimer::~SimpleTimer           ()
{
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

	m_startMillis = queryMillis();		// Must be after query frequency.
#else
	m_startMillis = timeGetTime();
#endif
}

// *********************************
//
void  SimpleTimer::Pause            ()
{
    if ( !m_paused )
    {
        m_paused = true;
        m_startPause = timeGetTime();
    }
}

// *********************************
//
void  SimpleTimer::UnPause         ()
{
    if ( m_paused )
    {
        m_paused = false;

		m_totalPausedTime += queryMillis() - m_startPause;
    }
}


unsigned long SimpleTimer::queryMillis() const
{
#ifdef QPF_TIMER
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );

	return unsigned long( currentTime.QuadPart / m_timerFrequency );
#else
	return timeGetTime();
#endif
}

} //bv
