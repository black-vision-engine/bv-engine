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
	
    void    StartTimer      ();
	void    StopTimer       ();
	
    double  CurElapsed      () const;
    double  GetElapsedTime  () const;

private:

    inline double  ToSecs          ( LARGE_INTEGER & time ) const;

};

}
