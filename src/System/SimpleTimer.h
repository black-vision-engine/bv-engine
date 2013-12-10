#pragma once

#include <windows.h>

#include "System/BasicTypes.h"

      
namespace bv
{

class SimpleTimer
{
private:

    bool            m_paused;
    unsigned long   m_startMillis;
    unsigned long   m_pausedTime;
    unsigned long   m_startPause;

public:

                            SimpleTimer     ();
                            ~SimpleTimer    ();

    void                    Start           ();
    void                    Pause           ();
    void                    UnPause         ();

    inline TimeType         ElapsedTime     () const;
    inline unsigned long    ElapsedMillis   () const;

};

// *********************************
//
TimeType         SimpleTimer::ElapsedTime     () const
{
    //FIXME: implement pause
    return TimeType( ElapsedMillis() ) * TimeType( 0.001 );
}

// *********************************
//
unsigned long    SimpleTimer::ElapsedMillis   () const
{
    //FIXME: implement pause
    return timeGetTime() - m_startMillis;
}

} //bv
