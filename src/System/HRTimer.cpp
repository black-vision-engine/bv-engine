#include "HRTimer.h"


namespace bv
{
	
// *********************************
//
HighResolutionTimer::HighResolutionTimer    ()
{
    Reinitialize();
}

// *********************************
//
void HighResolutionTimer::Reinitialize      ()
{
    m_StartTime.QuadPart    = 0;
    m_StopTime.QuadPart     = 0;

    QueryPerformanceFrequency( &m_Frequency );
}

// *********************************
//
void    HighResolutionTimer::StartTimer     ()
{
    QueryPerformanceCounter( &m_StartTime );
}

// *********************************
//
void    HighResolutionTimer::StopTimer      ()
{
    QueryPerformanceCounter( &m_StopTime );
}

// *********************************
//
double  HighResolutionTimer::GetElapsedTime () const
{
	LARGE_INTEGER time;

    time.QuadPart = m_StopTime.QuadPart - m_StartTime.QuadPart;

    return ToSecs( time );
}

} //bv
