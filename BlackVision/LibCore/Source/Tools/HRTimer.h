#pragma once

#include "win_sock.h"


namespace bv
{

class HighResolutionTimer {

private:

    LARGE_INTEGER m_StartTime;
    LARGE_INTEGER m_StopTime;

    LARGE_INTEGER m_Frequency;

    LARGE_INTEGER m_StartRe;
    LARGE_INTEGER m_StopRe;

public:
	
    HighResolutionTimer     ();

    void    Reinitialize    ();

    inline void    StartRe  ()
    {
        QueryPerformanceCounter( &m_StartRe );
    }

    inline void    StopRe   ()
    {
        QueryPerformanceCounter( &m_StopRe );
    }

    inline double    Re     () const
    {
        LARGE_INTEGER time;
        time.QuadPart = m_StopRe.QuadPart - m_StartRe.QuadPart;

        return ToSecs( time );
    }

    void    StartTimer      ();
	void    StopTimer       ();
	
    inline void Timestamp   ( LARGE_INTEGER * timestamp ) const;

    inline double CurElapsed( const LARGE_INTEGER & timestamp ) const;
    inline double CurElapsed() const;
    double  GetElapsedTime  () const;

private:

    inline double ToSecs    ( LARGE_INTEGER & time ) const;
    inline double Duration  ( const LARGE_INTEGER & tStart, const LARGE_INTEGER & tEnd ) const;

};

// *********************************
//
inline void HighResolutionTimer::Timestamp ( LARGE_INTEGER * timestamp ) const
{
    QueryPerformanceCounter( timestamp );
}

// *********************************
//
inline double HighResolutionTimer::CurElapsed( const LARGE_INTEGER & timestamp ) const
{
    LARGE_INTEGER curTime;
    QueryPerformanceCounter( &curTime );

    return Duration( timestamp, curTime );
}

// *********************************
//
inline double HighResolutionTimer::CurElapsed() const
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
