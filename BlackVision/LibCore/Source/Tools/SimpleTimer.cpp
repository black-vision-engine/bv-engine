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

    m_startMillis = timeGetTime();
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

        m_totalPausedTime += timeGetTime() - m_startPause;
    }
}

} //bv
