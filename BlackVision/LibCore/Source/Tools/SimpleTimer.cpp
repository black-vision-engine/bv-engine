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

	LARGE_INTEGER freq;

	QueryPerformanceFrequency( &freq );
	m_timerFrequency = (unsigned long)freq.QuadPart / 1000;

	m_startMillis = queryMillis();		// Must be after query frequency.
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
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );

	return unsigned long( currentTime.QuadPart / m_timerFrequency );
}

} //bv
