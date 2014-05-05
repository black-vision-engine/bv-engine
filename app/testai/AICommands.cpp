#include "AICommands.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"


namespace bv {

// ************************************************************ AI COMMAND BASE ************************************************************

// *********************************
//
AICommandBase::AICommandBase( TimeType triggerTime, const std::string & repr )
    : m_triggerTime( triggerTime )
    , m_lastTriggerTime( -1.f )
    , m_repr( repr )
{
}

// *********************************
//
TimeType    AICommandBase::LastTriggerTime () const
{
    return m_lastTriggerTime;
}

// *********************************
//
TimeType    AICommandBase::GetTriggerTime  () const
{
    return m_triggerTime;
}

// *********************************
//
bool        AICommandBase::TriggerCommand  ( TimeType t )
{
    if( std::abs( m_lastTriggerTime - t ) > TimeType( 0.1 ) )
    {
        m_lastTriggerTime = t;

        return TriggerImpl( t );
    }

    return false;
}

// *********************************
//
std::string AICommandBase::Repr            () const
{
    return m_repr;
}


// ************************************************************ AI COMMAND START ************************************************************

// *********************************
//
AICommandPlay::AICommandPlay    ( model::DefaultTimeline * timeline, TimeType triggerTime )
    : AICommandBase( triggerTime, "PLAY" )
    , m_timeline( timeline )
{
}

// *********************************
//
AICommandPlay::~AICommandPlay   ()
{
}

// *********************************
//
bool        AICommandPlay::TriggerImpl     ( TimeType t )
{
    m_timeline->Play();

    return true;
}


// ************************************************************ AI COMMAND STOP ************************************************************

// *********************************
//
AICommandStop::AICommandStop   ( model::DefaultTimeline * timeline, TimeType triggerTime )
    : AICommandBase( triggerTime, "STOP" )
    , m_timeline( timeline )
{
}

// *********************************
//
AICommandStop::~AICommandStop  ()
{
}

// *********************************
//
bool        AICommandStop::TriggerImpl     ( TimeType t )
{
    return true;
}

// ************************************************************ AI COMMAND REVERSE ************************************************************

// *********************************
//
AICommandReverse::AICommandReverse    ( model::DefaultTimeline * timeline, TimeType triggerTime )
    : AICommandBase( triggerTime, "REVERSE" )
    , m_timeline( timeline )
{
}

// *********************************
//
AICommandReverse::~AICommandReverse   ()
{
}

// *********************************
//
bool        AICommandReverse::TriggerImpl         ( TimeType t )
{
    return true;
}

} //bv
