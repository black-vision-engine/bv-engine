#pragma once

#include "CoreDEF.h"

#define QPF_TIMER

      
namespace bv
{

class SimpleTimer
{
private:

    bool            m_paused;
    unsigned long   m_startMillis;

    unsigned long   m_startPause;
    unsigned long   m_totalPausedTime;

#ifdef QPF_TIMER
	UInt64	        m_timerFrequency;
#endif
public:

                            SimpleTimer     ();
                            ~SimpleTimer    ();

    void                    Start           ();

    void                    Pause           ();
    void                    UnPause         ();

    inline TimeType         ElapsedTime     () const;
    inline unsigned long    ElapsedMillis   () const;

private:

	unsigned long           QueryMillis() const;

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


	return QueryMillis() - m_startMillis - m_totalPausedTime;
}

} //bv
