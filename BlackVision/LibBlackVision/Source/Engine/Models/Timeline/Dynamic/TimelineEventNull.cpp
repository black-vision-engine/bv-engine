#include "TimelineEventNull.h"
#include "Serialization/SerializationHelper.inl"

namespace bv { namespace model {

// *********************************
//
TimelineEventNull::TimelineEventNull   ( const std::string & name, TimeType eventTime, const ITimeline * owner )
    : Parent( name, TimelineEventType::TET_NULL, eventTime, owner )
{
}

// *********************************
//
TimelineEventNull::~TimelineEventNull  ()
{
}

// *********************************
//
void                TimelineEventNull::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "event" );
    ser.SetAttribute( "type", "null" );
    ser.SetAttribute( "name", GetName() );
    SerializationHelper::SerializeAttribute( ser, GetEventTime(), "time" );
ser.ExitChild();
}

// *********************************
//
TimelineEventNull*   TimelineEventNull::Create          ( const IDeserializer& deser, ITimeline* timeline )
{
    return new TimelineEventNull( deser.GetAttribute( "name" ),
        SerializationHelper::String2T< TimeType >( deser.GetAttribute( "time" ), 0.f ),
        timeline );
}

} //model
} //bv
