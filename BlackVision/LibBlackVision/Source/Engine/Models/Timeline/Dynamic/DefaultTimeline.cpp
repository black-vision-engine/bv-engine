#include "stdafx.h"

#include "DefaultTimeline.h"

#include <cassert>
#include <algorithm>

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Interfaces/ITimelineEvent.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventTrigger.h"

#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"
#undef min
#undef max



#include "Memory/MemoryLeaks.h"



namespace bv { 

namespace SerializationHelper {

std::pair< TimelineWrapMethod, const char* > TWM2S[] = {
    std::make_pair( TimelineWrapMethod::TWM_CLAMP, "clamp" ),
    std::make_pair( TimelineWrapMethod::TWM_MIRROR, "mirror" ),
    std::make_pair( TimelineWrapMethod::TWM_REPEAT, "repeat" ),
    std::make_pair( TimelineWrapMethod::TWM_CLAMP, "" ) };

template<> std::string                      T2String    ( const TimelineWrapMethod& twm )                               { return Enum2String( TWM2S, twm ); }
template<> TimelineWrapMethod               String2T    ( const std::string& s, const TimelineWrapMethod& defaultVal )  { return String2Enum( TWM2S, s, defaultVal ); }
template<> Expected< TimelineWrapMethod >   String2T    ( const std::string & s )                                       { return String2Enum( TWM2S, s ); }

} // SerializationHelper

namespace model {

namespace {

const TimeType GEvtTimeSeparation = TimeType( 0.2 ); //FIXME: some config (default) parameters should be used here - engine should expose it's connfiguration defaults for overriding

// *********************************
//
bool timelineEventComparator    ( const ITimelineEventPtr & e0, const ITimelineEventPtr & e1 )
{
    return e0->GetEventTime() < e1->GetEventTime();
}

} //anonymous


// *********************************
//
DefaultTimeline::DefaultTimeline     ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
    : Parent( name )
    , m_timeEvalImpl( duration, TimelinePlayDirection::TPD_FORWAD, preMethod, postMethod )
    , m_prevTime( TimeType( 0.0 ) )
    , m_activeEvent( nullptr )
{
}

// *********************************
//
DefaultTimeline::~DefaultTimeline    ()
{}

// *********************************
//
void                                DefaultTimeline::Serialize           ( ISerializer& ser ) const
{
    ser.EnterChild( "timeline" );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "type", "default" );

    SerializationHelper::SerializeAttribute( ser, m_timeEvalImpl.GetLocalTime(), "local_time" );
    SerializationHelper::SerializeAttribute( ser, m_timeEvalImpl.IsActive(), "play" );

    ser.SetAttribute( "duration", std::to_string( m_timeEvalImpl.GetDuration() ) );
    if( m_timeEvalImpl.GetWrapPre() == m_timeEvalImpl.GetWrapPost() )
    {
        ser.SetAttribute( "loop", SerializationHelper::T2String< TimelineWrapMethod >( m_timeEvalImpl.GetWrapPre() ) );
    }
    else
    {
        ser.SetAttribute( "loop", "true" );
        ser.SetAttribute( "loopPre", SerializationHelper::T2String< TimelineWrapMethod >( m_timeEvalImpl.GetWrapPre() ) );
        ser.SetAttribute( "loopPost", SerializationHelper::T2String< TimelineWrapMethod >( m_timeEvalImpl.GetWrapPost() ) );
    }

    ser.EnterArray( "events" );
    for( auto event : m_keyFrameEvents )
        event->Serialize( ser );
    ser.ExitChild();

    ser.EnterArray( "children" );
    for( auto child : m_children )
        child->Serialize( ser );
    ser.ExitChild(); // children

    ser.ExitChild();
}

// *********************************
//
DefaultTimelinePtr                  DefaultTimeline::Create     ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    return DefaultTimelinePtr( new DefaultTimeline( name, duration, preMethod, postMethod ) );
}

// *********************************
//
DefaultTimelinePtr                    DefaultTimeline::Create   ( const IDeserializer & deser )
{
    auto name = deser.GetAttribute( "name" );

    auto duration = SerializationHelper::String2T< float >( deser.GetAttribute( "duration" ), 777.f );

    auto loop = deser.GetAttribute( "loop" );
    TimelineWrapMethod preWrap, postWrap;
    if( loop == "true" )
    {
        preWrap = SerializationHelper::String2T< TimelineWrapMethod >( deser.GetAttribute( "loopPre" ) );
        postWrap = SerializationHelper::String2T< TimelineWrapMethod >( deser.GetAttribute( "loopPost" ) );
    }
    else
    {
        preWrap = postWrap = SerializationHelper::String2T< TimelineWrapMethod >( loop );
    }

    auto te = DefaultTimeline::Create( name, duration, preWrap, postWrap );

    if( deser.EnterChild( "events" ) )
    {
        if( deser.EnterChild( "event" ) )
        {
            do
            {
                auto type = deser.GetAttribute( "type" );
                if( type == "loop" )
                    te->AddKeyFrame( TimelineEventLoop::Create( deser, te.get() ) );
                else if( type == "null" )
                    te->AddKeyFrame( TimelineEventNull::Create( deser, te.get() ) );
                else if( type == "stop" )
                    te->AddKeyFrame( TimelineEventStop::Create( deser, te.get() ) );
                else
                    assert( false );

            } while( deser.NextChild() );

            deser.ExitChild(); // event
        }
        deser.ExitChild(); // events
    }

    auto children = SerializationHelper::DeserializeArray< TimeEvaluatorBase< ITimeEvaluator > >( deser, "children", "timeline" );

    for( auto child : children )
        te->AddChild( child );

    if( SerializationHelper::String2T< bool >( deser.GetAttribute( "play" ), false ) )
        te->Play();

    return te;
}

// *********************************
//
void								DefaultTimeline::SetDuration        ( TimeType duration )
{
    m_timeEvalImpl.SetDuration( duration );
}

// *********************************
//
TimeType                            DefaultTimeline::GetDuration        () const
{
    return m_timeEvalImpl.GetDuration();
}


// *********************************
//
void                                DefaultTimeline::SetGlobalTimeImpl  ( TimeType t )
{
    m_timeEvalImpl.UpdateGlobalTime( t );

    PostUpdateEventStep();

    auto prevTime = m_prevTime;
    m_prevTime = m_timeEvalImpl.GetLocalTime();

    TriggerEventStep( m_prevTime, prevTime );
}

// *********************************
//
TimeType                            DefaultTimeline::GetLocalTime       () const
{
    return m_timeEvalImpl.GetLocalTime();
}

// *********************************
//
void                                DefaultTimeline::SetLocalTime       ( TimeType t )
{
    m_timeEvalImpl.ResetLocalTimeTo( t );
}

// *********************************
//
void                                DefaultTimeline::Restart            ()
{
    m_timeEvalImpl.Reset();
}

// *********************************
//
void                                DefaultTimeline::SetPlayDirection   ( TimelinePlayDirection direction )
{
    if( m_timeEvalImpl.GetDirection() != direction )
    {
        m_timeEvalImpl.Reverse();
    }
}

// *********************************
//
TimelinePlayDirection               DefaultTimeline::GetPlayDirection   () const
{
    return m_timeEvalImpl.GetDirection();
}

// *********************************
//
void                                DefaultTimeline::Reverse            ()
{
    m_timeEvalImpl.Reverse();
}

// *********************************
//
void                                DefaultTimeline::Play               ()
{
    m_timeEvalImpl.Start();

    auto curTime = GetLocalTime();
    auto evt = CurrentEvent( curTime, m_prevTime );

    if( evt )
    {
        DeactivateEvent( evt );
    }
}

// *********************************
//
void                                DefaultTimeline::Stop               ()
{
    m_timeEvalImpl.Stop();
}

// *********************************
//
void                                DefaultTimeline::SetTimeAndStop     ( TimeType t )
{
    m_prevTime = t;
    m_timeEvalImpl.ResetLocalTimeTo( t );

    PostUpdateEventStep();

    Stop();
}

// *********************************
//
void                                DefaultTimeline::SetTimeAndPlay     ( TimeType t )
{
    m_prevTime = t;
    m_timeEvalImpl.ResetLocalTimeTo( t );

    PostUpdateEventStep();

    Play();
}

// *********************************
//
void                                DefaultTimeline::SetWrapBehavior    ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    m_timeEvalImpl.SetWrapBehavior( preMethod, postMethod );
}

// *********************************
//
TimelineWrapMethod                  DefaultTimeline::GetWrapBehaviorPre () const
{
    return m_timeEvalImpl.GetWrapPre();
}

// *********************************
//
TimelineWrapMethod                  DefaultTimeline::GetWrapBehaviorPost() const
{
    return m_timeEvalImpl.GetWrapPost();
}

// *********************************
//
SizeType                            DefaultTimeline::NumKeyFrames       () const
{
    return m_keyFrameEvents.size();
}

// *********************************
//
bool                                DefaultTimeline::AddKeyFrame        ( const ITimelineEventPtr & evt )
{
    if( !CanBeInserted( evt.get() ) )
    {
        return false;
    }

    m_keyFrameEvents.push_back( evt );

    std::sort( m_keyFrameEvents.begin(), m_keyFrameEvents.end(), timelineEventComparator );

    return true;
}

// *********************************
//
const ITimelineEvent *              DefaultTimeline::GetKeyFrameEvent   ( const std::string & name ) const
{
    for( auto e : m_keyFrameEvents )
    {
        if ( e->GetName() == name )
        {
            return e.get();
        }
    }

    return nullptr;
}

// *********************************
//
const ITimelineEvent *              DefaultTimeline::GetKeyFrameEvent   ( unsigned int idx ) const
{
    if( idx >= m_keyFrameEvents.size() )
    {
        return nullptr;
    }

    return m_keyFrameEvents[ idx ].get();
}

// *********************************
//
bool                                DefaultTimeline::RemoveKeyFrameEvent( unsigned int idx )
{
    assert( idx < m_keyFrameEvents.size() );

    auto evn = m_keyFrameEvents[ idx ];
    if( evn.get() == m_activeEvent )
        m_activeEvent = nullptr;

    m_keyFrameEvents.erase( m_keyFrameEvents.begin() + idx );


    return true;
}

// *********************************
//
bool                                DefaultTimeline::RemoveKeyFrameEvent( const std::string & name )
{
    for( auto it = m_keyFrameEvents.begin(); it != m_keyFrameEvents.end(); ++it )
    {
        if ( (*it)->GetName() == name )
        {
            auto evn = *it;
            if( evn.get() == m_activeEvent )
                m_activeEvent = nullptr;


            m_keyFrameEvents.erase( it );

            return true;
        }
    }

    return false;
}

// *********************************
//
void                                DefaultTimeline::DeactivateEvent    ( ITimelineEvent * evt )
{
    assert( m_activeEvent == evt || m_activeEvent == nullptr );

    if( m_activeEvent != evt )
    {
        //printf( "Deactivating %s \n", evt->GetName().c_str() );

        m_activeEvent = evt;
        m_activeEvent->SetActive( false ); //FIXME: not necessarily useful
    }
}

// *********************************
//
ITimelineEvent *                    DefaultTimeline::CurrentEvent       ( TimeType curTime, TimeType prevTime ) const
{
    if( m_timeEvalImpl.IsActive() )
    {        
        auto t0 = std::min( curTime, prevTime );
        auto t1 = std::max( curTime, prevTime );

        for( auto evt : m_keyFrameEvents )
        {
            auto eventTime = evt->GetEventTime();

            if( eventTime >= t0 && eventTime <= t1 )
            {
                return evt.get();
            }
        }
    }

    return nullptr;
}

// *********************************
//
bool                                DefaultTimeline::CanBeInserted      ( const ITimelineEvent * evt ) const
{
    if ( evt == nullptr )
    {
        return false;
    }

    for( auto e : m_keyFrameEvents )
    {
        if( std::abs( e->GetEventTime() - evt->GetEventTime() ) < GEvtTimeSeparation )
        {
            return false;
        }
    }

    return true;
}

// *********************************
//
void                                DefaultTimeline::TriggerEventStep       ( TimeType curTime, TimeType prevTime )
{
    auto evt = CurrentEvent( curTime, prevTime );

    if( evt == nullptr || evt == m_activeEvent || !evt->IsActive() )
    {
        return;
    }

    assert( m_activeEvent == nullptr );

    DeactivateEvent( evt );

    TimelineKeyframeEvent::KeyframeType keyframeType = TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail;
    switch( evt->GetType() )
    {
        case TimelineEventType::TET_STOP:
        {
            evt->SetActive( false );
            SetLocalTime( evt->GetEventTime() );
            m_timeEvalImpl.Stop();
            //printf( "Event STOP %s prev: %.4f, cur: %.4f\n", evt->GetName().c_str(), prevTime, curTime );
            //printf( "Event STOP %s \n", evt->GetName().c_str() );
            
            keyframeType = TimelineKeyframeEvent::KeyframeType::StopKeyframe;

            break;
        }
        case TimelineEventType::TET_LOOP:
        {
            auto evtImpl = static_cast< TimelineEventLoop * >( evt );

            switch( evtImpl->GetActionType() )
            {
                case LoopEventAction::LEA_GOTO:
                    assert( std::abs( GetLocalTime() - evtImpl->GetTargetTime() ) > GEvtTimeSeparation );

                    m_timeEvalImpl.ResetLocalTimeTo( evtImpl->GetTargetTime() );

                    m_prevTime = evtImpl->GetTargetTime();

                    m_activeEvent->SetActive( true ); //Reset current event (forthcomming play will trigger its set of event problems - e.g. first event at 0.0 to pass by).
                    m_activeEvent = nullptr;

                    keyframeType = TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe;

                    //printf( "Event LOOP -> GOTO: %.4f %s\n", evtImpl->GetTargetTime(), evt->GetName().c_str() );
                    break;
                case LoopEventAction::LEA_RESTART:
                    m_timeEvalImpl.Reset();

                    m_prevTime = TimeType( 0.0 );

                    m_activeEvent->SetActive( true ); //Reset current event (forthcomming play will trigger its set of event problems - e.g. first event at 0.0 to pass by).
                    m_activeEvent = nullptr;

                    Play(); //FIXME: really start or should we wait for the user to trigger this timeline?

                    keyframeType = TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe;

                    //printf( "Event LOOP -> RESTART %s\n", evt->GetName().c_str() );                    
                    break;
                case LoopEventAction::LEA_REVERSE:
                    Reverse();
                    
                    keyframeType = TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe;

                    //printf( "Event LOOP -> REVERSE %s\n", evt->GetName().c_str() );
                    
                    break;
                default:
                    assert( false );
            }

            evtImpl->IncLoopCount();
                
            //printf( "Event LOOP %d\n", evtImpl->GetLoopCount() );

            break;
        }
        case TimelineEventType::TET_TRIGGER:
        {
            auto evtImpl = static_cast< TimelineEventTrigger * >( evt );
            evtImpl->SetActive( false );
            m_activeEvent = nullptr;

            evtImpl->SendEvents();

            keyframeType = TimelineKeyframeEvent::KeyframeType::TriggerEventKeyframe;

            //printf( "Event Trigger\n" );

            break;
        }
        case TimelineEventType::TET_NULL:
        {
            auto evtImpl = static_cast< TimelineEventNull * >( evt );
            evtImpl->SetActive( false );

            keyframeType = TimelineKeyframeEvent::KeyframeType::NullKeyframe;

            //printf( "Event NULL\n" );

            break;
        }
        default:
            assert( false );
    }

    JsonSerializeObject ser;
	ser.SetAttribute( "TriggerEvent", "KeyframeTrigger" );
    ser.SetAttribute( "cmd", "KeyframeEvent" );
    ser.SetAttribute( "KeyframeType", SerializationHelper::T2String( keyframeType ) );
    ser.SetAttribute( "KeyframeName", evt->GetName() );
    ser.SetAttribute( "Timeline", this->GetName() );
    ser.SetAttribute( "SceneName", TimelineHelper::GetSceneName( this ) );
    ser.SetAttribute( "Time", SerializationHelper::T2String( this->GetLocalTime() ) );

    ResponseEventPtr msg = std::make_shared<ResponseEvent>();
    msg->Response = ser.GetString();
    msg->SocketID = SEND_BROADCAST_EVENT;
    GetDefaultEventManager().QueueResponse( msg );
}

// *********************************
//
void                                DefaultTimeline::PostUpdateEventStep    ()
{
    if( m_activeEvent && !m_activeEvent->IsActive() )
    {
        auto t = GetLocalTime();
     
        if( std::abs( t - m_activeEvent->GetEventTime() ) > GEvtTimeSeparation * 0.5f )
        {
            //printf( "Activating: %s at %.4f evtt: %.4f\n", m_activeEvent->GetName().c_str(), t, m_activeEvent->GetEventTime() );
            m_activeEvent->SetActive( true );
            m_activeEvent = nullptr;
        }
    }
}

// ***********************
//
const std::string&        DefaultTimeline::GetType             ()
{
    return Type();
}

// ***********************
//
namespace {
const std::string DefaultTimelineType = "DefaultTimeline";
} //annonymous

// ***********************
//
const std::string&        DefaultTimeline::Type                ()
{
    return DefaultTimelineType;
}

} //model
} //bv
