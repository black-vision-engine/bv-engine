#include "TimelineEventLoop.h"
#include "Serialization/SerializationHelper.inl"

namespace bv { 
    
namespace SerializationHelper {

std::pair< LoopEventAction, const char* > lea2s[] =
{ std::make_pair( LoopEventAction::LEA_GOTO, "goto" )
, std::make_pair( LoopEventAction::LEA_RESTART, "restart" )
, std::make_pair( LoopEventAction::LEA_REVERSE, "reverse" )
, std::make_pair( LoopEventAction::LEA_TOTAL, "" ) };

template<> std::string T2String( const LoopEventAction& lea ) { return Enum2String( lea2s, lea ); }

}
    
namespace model {

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
TimelineEventLoop* TimelineEventLoop::Create            ( const IDeserializer& deser, ITimeline* timeline )
{
    return new TimelineEventLoop( deser.GetAttribute( "name" ),
        SerializationHelper::String2T< TimeType >( "time", 0.f ),
        SerializationHelper::String2T< LoopEventAction >( nullptr, "action" ),
        SerializationHelper::String2T< unsigned int >( "loopCount", 0 ),
        SerializationHelper::String2T< TimeType >( "targetTime", 0.f ),
        timeline );
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
