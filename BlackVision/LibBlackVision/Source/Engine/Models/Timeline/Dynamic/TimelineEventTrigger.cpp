#include "stdafx.h"
#include "TimelineEventTrigger.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/EventManager.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model
{


// ***********************
//
TimelineEventTrigger::TimelineEventTrigger( const std::string & name, TimeType eventTime, const ITimeline * owner, const std::string & eventsToTrigger )
    :   Parent( name, TimelineEventType::TET_TRIGGER, eventTime, owner )
    ,   m_eventsToTrigger( eventsToTrigger )
{}

// ***********************
//
TimelineEventTrigger::~TimelineEventTrigger()
{}

// ***********************
//
void    TimelineEventTrigger::Serialize       ( ISerializer & ser ) const
{
ser.EnterChild( "event" );
    ser.SetAttribute( "type", "trigger" );
    ser.SetAttribute( "name", GetName() );

    ser.SetAttribute( "events", m_eventsToTrigger );

ser.ExitChild();
}

// ***********************
//
TimelineEventTriggerPtr     TimelineEventTrigger::Create          ( const IDeserializer & deser, const ITimeline * timeline )
{
    std::string     name = deser.GetAttribute( "name" );
    TimeType        time = Convert::String2T< TimeType >( "time", 0.f );
    std::string     events = deser.GetAttribute( "events" );

    return std::make_shared< TimelineEventTrigger >( name, time, timeline, events );
}

TimelineEventTriggerPtr     TimelineEventTrigger::Create          ( const std::string & name, TimeType eventTime, const std::string & eventsToTrigger, const ITimeline * owner )
{
    return std::make_shared< TimelineEventTrigger >( name, eventTime, owner, eventsToTrigger );
}

// ***********************
//
void    TimelineEventTrigger::SendEvents      ()
{
    auto eventsVec = GetDefaultEventManager().GetEventFactory().ParseEventsListJSON( m_eventsToTrigger, SEND_BROADCAST_EVENT );

    for( auto & evt : eventsVec )
    {
        GetDefaultEventManager().QueueEvent( evt );
    }
}

}   // model
}	// bv
