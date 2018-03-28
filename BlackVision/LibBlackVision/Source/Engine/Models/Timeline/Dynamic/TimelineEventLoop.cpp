#include "stdafx.h"

#include "TimelineEventLoop.h"
#include "Serialization/SerializationHelper.inl"



#include "Memory/MemoryLeaks.h"



namespace bv {
namespace Convert
{

std::pair< LoopEventAction, const char* > lea2s[] =
{ std::make_pair( LoopEventAction::LEA_GOTO, "goto" )
, std::make_pair( LoopEventAction::LEA_RESTART, "restart" )
, std::make_pair( LoopEventAction::LEA_REVERSE, "reverse" )
, std::make_pair( LoopEventAction::LEA_TOTAL, "" ) };

IMPLEMENT_ENUM_SERIALIZATION( LoopEventAction, lea2s );

}   // Convert


namespace model 
{

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
void                TimelineEventLoop::Serialize       ( ISerializer& ser ) const
{
ser.EnterChild( "event" );
    ser.SetAttribute( "type", "loop" );
    ser.SetAttribute( "name", GetName() );
    SerializationHelper::SerializeAttribute( ser, GetEventTime(), "time" );
    SerializationHelper::SerializeAttribute( ser, GetActionType(), "action" );
    SerializationHelper::SerializeAttribute( ser, m_totalLoopCount, "loopCount" );
    SerializationHelper::SerializeAttribute( ser, m_targetTime, "targetTime" );
ser.ExitChild();
}

// *********************************
//
TimelineEventLoopPtr TimelineEventLoop::Create          ( const std::string & name, TimeType eventTime, LoopEventAction action, unsigned int totalLoopCount, TimeType targetTime, const ITimeline * owner )
{
    return TimelineEventLoopPtr( new TimelineEventLoop( name, eventTime, action, totalLoopCount, targetTime, owner ) );
}


// *********************************
//
TimelineEventLoopPtr TimelineEventLoop::Create          ( const IDeserializer & deser, const ITimeline * timeline )
{
    auto name = deser.GetAttribute( "name" );

    auto time = Convert::String2T< TimeType >( deser.GetAttribute( "time" ) );
    if( !time.IsValid() )
    {
        Warn< SerializationException >( deser,
            "Cannot deserialize loop keyframe's time. Keyframe's name: " + name + ", timeline's name: " + timeline->GetName() +
            ". Keyframe skipped." );
        return nullptr;
    }

    auto action = Convert::String2T< LoopEventAction >( deser.GetAttribute( "action" ) );
    if( !action.IsValid() )
    {
        Warn< SerializationException >( deser,
            "Cannot deserialize loop keyframe's action. Keyframe's name: " + name + ", timeline's name: " + timeline->GetName() +
            ". Keyframe skipped." );
        return nullptr;
    }

    auto loopCount = Convert::String2T< unsigned int >( deser.GetAttribute( "loopCount" ) );
    if( !action.IsValid() )
    {
        Warn< SerializationException >( deser,
            "Cannot deserialize loop keyframe's loop count. Keyframe's name: " + name + ", timeline's name: " + timeline->GetName() +
            ". Loop count set to max." );
        loopCount = std::numeric_limits< unsigned int >::max();
    }

    auto targetTime = Convert::String2T< TimeType >( deser.GetAttribute( "targetTime" ) );
    if( !targetTime.IsValid() )
        if( action == LoopEventAction::LEA_GOTO )
        {
            Warn< SerializationException >( deser,
                "Cannot deserialize goto keyframe's target time. Keyframe's name: " + name + ", timeline's name: " + timeline->GetName() +
                ". Keyframe skipped." );
            return nullptr;
        }
        else
        {
            Warn< SerializationException >( deser,
                "Cannot deserialize loop keyframe's target time. Keyframe's name: " + name + ", timeline's name: " + timeline->GetName() +
                ". Target time set to 0." );
            targetTime = 0;
        }


    return TimelineEventLoop::Create( name, time, action, loopCount, targetTime, timeline );
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
    m_targetTime = t;
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
