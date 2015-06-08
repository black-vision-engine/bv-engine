#pragma once

#include <windows.h>

#include "CoreDEF.h"

      
namespace bv
{

class SimpleTimer
{
private:

    bool            m_paused;
    unsigned long   m_startMillis;

    unsigned long   m_startPause;
    unsigned long   m_totalPausedTime;

	unsigned long	m_timerFrequency;

public:

                            SimpleTimer     ();
                            ~SimpleTimer    ();

    void                    Start           ();

    void                    Pause           ();
    void                    UnPause         ();

    inline TimeType         ElapsedTime     () const;
    inline unsigned long    ElapsedMillis   () const;

private:

	unsigned long queryMillis() const;

    inline TimeType         MillisToTime    ( unsigned long millis ) const
    {
        return TimeType( millis ) * TimeType( 0.001 );
    }

};

// *********************************
//
TimeType         SimpleTimer::ElapsedTime     () const
{
    return MillisToTime( ElapsedMillis() );
}

// *********************************
//
unsigned long    SimpleTimer::ElapsedMillis   () const
{
    if( m_paused )
    {
        return m_startPause - m_startMillis - m_totalPausedTime;
    }


	return queryMillis() - m_startMillis - m_totalPausedTime;
}

} //bv
