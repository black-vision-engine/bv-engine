#pragma once

#include <windows.h>


namespace bv
{

class HighResolutionTimer {

private:

    LARGE_INTEGER m_StartTime;
    LARGE_INTEGER m_StopTime;

    LARGE_INTEGER m_Frequency;

public:
	
    HighResolutionTimer     ();

    void    Renitialize     ();

    void    StartTimer      ();
	void    StopTimer       ();
	
    inline double CurElapsed() const;
    double  GetElapsedTime  () const;

private:

    inline double ToSecs    ( LARGE_INTEGER & time ) const;
    inline double Duration  ( const LARGE_INTEGER & tStart, const LARGE_INTEGER & tEnd ) const;

};

// *********************************
//
inline double  HighResolutionTimer::CurElapsed() const
{
    LARGE_INTEGER curTime;
    QueryPerformanceCounter( &curTime );

    LARGE_INTEGER time;
    time.QuadPart = curTime.QuadPart - m_StartTime.QuadPart;

    return ToSecs( time );
}

// *********************************
//
inline double  HighResolutionTimer::ToSecs  ( LARGE_INTEGER & time ) const
{
    return ( (double) time.QuadPart / (double) m_Frequency.QuadPart );
}

// *********************************
//
inline double HighResolutionTimer::Duration ( const LARGE_INTEGER & tStart, const LARGE_INTEGER & tEnd ) const
{
    return ( (double) ( tEnd.QuadPart - tStart.QuadPart ) / (double) m_Frequency.QuadPart );

}

} //bv
