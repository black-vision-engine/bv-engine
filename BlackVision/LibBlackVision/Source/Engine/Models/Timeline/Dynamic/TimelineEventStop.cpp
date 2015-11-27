#include "TimelineEventStop.h"
#include "Serialization/SerializationHelper.inl"

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
void                TimelineEventStop::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "event" );
    ser.SetAttribute( "type", "stop" );
    SerializationHelper::SerializeAttribute( ser, GetEventTime(), "time" );
ser.ExitChild();
}

// *********************************
//
TimelineEventStop*   TimelineEventStop::Create          ( const IDeserializer& deser, ITimeline* timeline )
{
    return new TimelineEventStop( deser.GetAttribute( "name" ),
        SerializationHelper::String2T< TimeType >( deser.GetAttribute( "time" ), 0.f ),
        timeline );
}

} //model
} //bv
