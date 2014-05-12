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
    if( std::abs( m_lastTriggerTime - t ) > TimeType( 0.15 ) )
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

// *********************************
//
void        AICommandBase::SetRepr         ( const std::string & repr )
{
    m_repr = repr;
}


// ************************************************************ AI COMMAND START ************************************************************

// *********************************
//
AICommandPlay::AICommandPlay    ( model::DefaultTimelinePtr timeline, TimeType triggerTime )
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
void        AICommandPlay::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
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
AICommandStop::AICommandStop   ( model::DefaultTimelinePtr timeline, TimeType triggerTime )
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
void        AICommandStop::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandStop::TriggerImpl     ( TimeType t )
{
    m_timeline->Stop();

    return true;
}

// ************************************************************ AI COMMAND REVERSE ************************************************************

// *********************************
//
AICommandReverse::AICommandReverse    ( model::DefaultTimelinePtr timeline, TimeType triggerTime )
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
void        AICommandReverse::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandReverse::TriggerImpl         ( TimeType t )
{
    m_timeline->Reverse();

    return true;
}


// ************************************************************ AI COMMAND SET PLAY DIRECTION ************************************************************

// *********************************
//
AICommandSetPlayDirection::AICommandSetPlayDirection    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimelinePlayDirection direction )
    : AICommandBase( triggerTime, "" )
    , m_timeline( timeline )
    , m_direction( direction )
{
    if( direction == TimelinePlayDirection::TPD_FORWAD )
    {
        SetRepr( std::string( "SET PLAY DIRECTION " ) + std::string( "FORWARD" ) );
    }
    else
    {
        SetRepr( std::string( "SET PLAY DIRECTION " ) + std::string( "BACKWARD" ) );
    }
}

// *********************************
//
AICommandSetPlayDirection::~AICommandSetPlayDirection   ()
{
}

// *********************************
//
void        AICommandSetPlayDirection::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandSetPlayDirection::TriggerImpl      ( TimeType t )
{
    m_timeline->SetPlayDirection( m_direction );

    return true;
}

// ************************************************************ AI COMMAND SET TIME AND STOP ************************************************************

// *********************************
//
AICommandSetTimeAndStop::AICommandSetTimeAndStop    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimeType eventTime )
    : AICommandBase( triggerTime, "" )
    , m_timeline( timeline )
    , m_eventTime( eventTime )
{
    SetRepr( std::string( "SET TIME AND STOP -> " ) + std::to_string( eventTime ) );
}

// *********************************
//
AICommandSetTimeAndStop::~AICommandSetTimeAndStop   ()
{
}

// *********************************
//
void        AICommandSetTimeAndStop::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandSetTimeAndStop::TriggerImpl    ( TimeType t )
{
    m_timeline->SetTimeAndStop( m_eventTime );

    return true;
}


// ************************************************************ AI COMMAND SET TIME AND PLAY ************************************************************

// *********************************
//
AICommandSetTimeAndPlay::AICommandSetTimeAndPlay    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimeType eventTime )
    : AICommandBase( triggerTime, "" )
    , m_timeline( timeline )
    , m_eventTime( eventTime )
{
    SetRepr( std::string( "SET TIME AND PLAY -> " ) + std::to_string( eventTime ) );
}

// *********************************
//
AICommandSetTimeAndPlay::~AICommandSetTimeAndPlay   ()
{
}

// *********************************
//
void        AICommandSetTimeAndPlay::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandSetTimeAndPlay::TriggerImpl    ( TimeType t )
{
    m_timeline->SetTimeAndPlay( m_eventTime );

    return true;
}

} //bv
