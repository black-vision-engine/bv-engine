#include "HRTimer.h"

namespace bv
{
	
HighResolutionTimer::HighResolutionTimer     ()
{
    m_StartTime.QuadPart    = 0;
    m_StopTime.QuadPart     = 0;

    QueryPerformanceFrequency( &m_Frequency );
}
	
// *********************************
//
void    HighResolutionTimer::StartTimer      ()
{
    QueryPerformanceCounter( &m_StartTime );
}

// *********************************
//
void    HighResolutionTimer::StopTimer       ()
{
    QueryPerformanceCounter( &m_StopTime );
}

// *********************************
//
double  HighResolutionTimer::GetElapsedTime  () const
{
	LARGE_INTEGER time;

    time.QuadPart = m_StopTime.QuadPart - m_StartTime.QuadPart;

    return ToSecs( time );
}

// *********************************
//
double  HighResolutionTimer::CurElapsed      () const
{
    LARGE_INTEGER curTime;
    QueryPerformanceCounter( &curTime );

    LARGE_INTEGER time;
    time.QuadPart = curTime.QuadPart - m_StartTime.QuadPart;

    return ToSecs( time );
}

// *********************************
//
inline double  HighResolutionTimer::ToSecs          ( LARGE_INTEGER & time ) const
{
    return ( (double) time.QuadPart / (double) m_Frequency.QuadPart );
}

}
