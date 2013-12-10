#include "SimpleTimer.h"


namespace bv
{

// *********************************
//
SimpleTimer::SimpleTimer            ()
    : m_pausedTime( 0 )
    , m_paused( false )
    , m_startPause( 0 )
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
    //FIXME: implement
}

} //bv
