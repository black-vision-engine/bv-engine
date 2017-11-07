#include "stdafx.h"

#include "TimelineEventStop.h"
#include "Serialization/SerializationHelper.inl"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *********************************
//
TimelineEventStop::TimelineEventStop   ( const std::string & name, TimeType eventTime, const ITimeline * owner )
    : Parent( name, TimelineEventType::TET_STOP, eventTime, owner )
{
}

// *********************************
//
TimelineEventStop::~TimelineEventStop  ()
{
}

// *********************************
//
void                    TimelineEventStop::Serialize        ( ISerializer & ser ) const
{
    ser.EnterChild( "event" );
    ser.SetAttribute( "type", "stop" );
    ser.SetAttribute( "name", GetName() );
    SerializationHelper::SerializeAttribute( ser, GetEventTime(), "time" );
    ser.ExitChild(); // "event"
}

// *********************************
//
TimelineEventStopPtr    TimelineEventStop::Create           ( const std::string & name, TimeType eventTime, const ITimeline * owner )
{
    return TimelineEventStopPtr( new TimelineEventStop( name, eventTime, owner ) );
}

// *********************************
//
TimelineEventStopPtr    TimelineEventStop::Create           ( const IDeserializer & deser, const ITimeline * timeline )
{
    return TimelineEventStop::Create( deser.GetAttribute( "name" ),
        Convert::String2T< TimeType >( deser.GetAttribute( "time" ), 0.f ),
        timeline );
}

} //model
} //bv
