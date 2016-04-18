#include "stdafx.h"
#include "TimelineEventTrigger.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/EventManager.h"
#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"

namespace bv { namespace model
{


// ***********************
//
TimelineEventTrigger::TimelineEventTrigger( const std::string & name, TimeType eventTime, const ITimeline * owner, std::vector< IEventPtr >&& eventsToTrigger )
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

    ser.EnterArray( "EventsToTrigger" );
    for( auto& evt : m_eventsToTrigger )
    {
        evt->Serialize( ser );
    }
    ser.ExitChild();

ser.ExitChild();
}

// ***********************
//
TimelineEventTriggerPtr     TimelineEventTrigger::Create          ( const IDeserializer & deser, const ITimeline * timeline )
{
    std::string name = deser.GetAttribute( "name" );
    TimeType    time = SerializationHelper::String2T< TimeType >( "time", 0.f );
    std::vector< IEventPtr >    triggers;

    //if( deser.EnterChild( "EventsToTrigger" ) )
    //{
    //    if( deser.EnterChild( "Event" ) )
    //    {
    //        do
    //        {
    //            auto newEvent = GetDefaultEventManager().GetEventFactory().DeserializeEvent( deser );
    //            if( newEvent != nullptr )
    //            {
    //                RemoteEventPtr newEventBase = std::static_pointer_cast< RemoteEvent >( newEvent );
    //                newEventBase->SocketID = SEND_BROADCAST_EVENT;
    //                newEventBase->EventID = SerializationHelper::String2T( deser.GetAttribute( "EventID" ), std::numeric_limits< int >::max() );

    //                triggers.push_back( newEventBase );
    //            }

    //        } while( deser.NextChild() );
    //    }
    //}

    return std::make_shared< TimelineEventTrigger >( name, time, timeline, std::move( triggers ) );
}

// ***********************
//
void    TimelineEventTrigger::SendEvents      ()
{
    for( auto& evt : m_eventsToTrigger )
    {
        GetDefaultEventManager().QueueEvent( evt/*->Clone()*/ );
    }
}

}   // model
}	// bv
