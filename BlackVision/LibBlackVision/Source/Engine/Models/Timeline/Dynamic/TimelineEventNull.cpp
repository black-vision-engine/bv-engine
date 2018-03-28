#include "stdafx.h"

#include "TimelineEventNull.h"
#include "Serialization/SerializationHelper.inl"



#include "Memory/MemoryLeaks.h"



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
void                    TimelineEventNull::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "event" );
    ser.SetAttribute( "type", "null" );
    ser.SetAttribute( "name", GetName() );
    SerializationHelper::SerializeAttribute( ser, GetEventTime(), "time" );
    ser.ExitChild(); // event
}

// *********************************
//
TimelineEventNullPtr     TimelineEventNull::Create         ( const std::string & name, TimeType eventTime, const ITimeline * owner )
{
    return TimelineEventNullPtr( new TimelineEventNull( name, eventTime, owner ) );
}

// *********************************
//
TimelineEventNullPtr    TimelineEventNull::Create          ( const IDeserializer & deser, const ITimeline * timeline )
{
    auto name = deser.GetAttribute( "name" );
    
    auto            time = Convert::String2T< TimeType >( deser.GetAttribute( "time" ) );
    if( !time.IsValid() )
    {
        Warn< SerializationException >( deser,
            "Cannot deserialize null keyframe's time. Keyframe's name: " + name + ", timeline's name: " + timeline->GetName() +
            ". Keyframe skipped." );
        return nullptr;
    }

    return TimelineEventNull::Create( name, time, timeline );
}

} //model
} //bv
