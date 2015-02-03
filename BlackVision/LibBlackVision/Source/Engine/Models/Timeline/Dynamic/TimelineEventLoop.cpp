#include "TimelineEventLoop.h"


namespace bv { namespace model {

// *********************************
//
TimelineEventLoop::TimelineEventLoop   ( const std::string & name, TimeType eventTime, LoopEventAction action, unsigned int totalLoopCount, TimeType targetTime, const ITimeline * owner )
    : Parent( name, TimelineEventType::TET_LOOP, eventTime, owner )
    , m_action( action )
    , m_curLoopCount( 0 )
    , m_totalLoopCount( totalLoopCount )
    , m_targetTime( targetTime )
{
}

// *********************************
//
TimelineEventLoop::~TimelineEventLoop  ()
{
}

// *********************************
//
unsigned int    TimelineEventLoop::GetLoopCount        () const
{
    return m_curLoopCount;
}

// *********************************
//
unsigned int    TimelineEventLoop::GetTotalLoopCount   () const
{
    return m_totalLoopCount;
}

// *********************************
//
LoopEventAction TimelineEventLoop::GetActionType       () const
{
    return m_action;
}

// *********************************
//
TimeType        TimelineEventLoop::GetTargetTime       () const
{
    return m_targetTime;
}

// *********************************
//
bool            TimelineEventLoop::IsActive            () const
{
    return Parent::IsActive() && ( GetLoopCount() < GetTotalLoopCount() );
}

// *********************************
//
void            TimelineEventLoop::SetTargetTime       ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    m_targetTime = m_targetTime;
}

// *********************************
//
void            TimelineEventLoop::SetAction           ( LoopEventAction action )
{
    m_action = action;
}

// *********************************
//
void            TimelineEventLoop::SetLoopCount        ( unsigned int loopCount )
{
    m_curLoopCount = loopCount;
}

// *********************************
//
void            TimelineEventLoop::IncLoopCount        ()
{
    m_curLoopCount++;
}

// *********************************
//
void            TimelineEventLoop::ResetLoopCount      ()
{
    m_curLoopCount = 0;
}

// *********************************
//
void            TimelineEventLoop::SetTotalLoopCount   ( unsigned int totalLoopCount )
{
    m_totalLoopCount = totalLoopCount;
}

} //model
} //bv
