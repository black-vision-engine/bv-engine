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



#include "UseLoggerLibBlackVision.h"



namespace bv { 

namespace Convert
{

std::pair< TimelineWrapMethod, const char* > TWM2S[] = {
    std::make_pair( TimelineWrapMethod::TWM_CLAMP, "clamp" ),
    std::make_pair( TimelineWrapMethod::TWM_MIRROR, "mirror" ),
    std::make_pair( TimelineWrapMethod::TWM_REPEAT, "repeat" ),
    std::make_pair( TimelineWrapMethod::TWM_CLAMP, "" ) };

template<> std::string                      T2String    ( const TimelineWrapMethod& twm )                               { return Enum2String( TWM2S, twm ); }
template<> TimelineWrapMethod               String2T    ( const std::string& s, const TimelineWrapMethod& defaultVal )  { return String2Enum( TWM2S, s, defaultVal ); }
template<> Expected< TimelineWrapMethod >   String2T    ( const std::string & s )                                       { return String2Enum( TWM2S, s ); }

} // Convert

namespace model {

namespace {

const TimeType GEvtTimeSeparation = TimeType( 0.02 ); //FIXME: some config (default) parameters should be used here - engine should expose it's connfiguration defaults for overriding
const TimeType GEpsilon = 0.00001f;


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
    , m_prevLocalTime( TimeType( 0.0 ) )
    , m_prevGlobalTime( TimeType( 0.0 ) )
    , m_triggeredEvent( nullptr )
    , m_triggeredPlay( true )
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
        ser.SetAttribute( "loop", Convert::T2String< TimelineWrapMethod >( m_timeEvalImpl.GetWrapPre() ) );
    }
    else
    {
        ser.SetAttribute( "loop", "true" );
        ser.SetAttribute( "loopPre", Convert::T2String< TimelineWrapMethod >( m_timeEvalImpl.GetWrapPre() ) );
        ser.SetAttribute( "loopPost", Convert::T2String< TimelineWrapMethod >( m_timeEvalImpl.GetWrapPost() ) );
    }

//    SerializeChildren( ser );

    ser.EnterArray( "events" );
    for( auto event : m_keyFrameEvents )
        event->Serialize( ser );
    ser.ExitChild();

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

    auto duration = Convert::String2T< TimeType >( deser.GetAttribute( "duration" ) );
    if( !duration.IsValid() )
    {
        Warn< SerializationException >( deser, "Cannot deserialize timeline duration. Timeline's name: " + name + ". Replacing with maximal duration." );
        duration = std::numeric_limits< TimeType >::max(); // NOTE: perhaps infinity() would be better?
    }

    auto loop = deser.GetAttribute( "loop" );
    Expected< TimelineWrapMethod > preWrap, postWrap;
    if( loop == "true" )
    {
        preWrap = Convert::String2T< TimelineWrapMethod >( deser.GetAttribute( "loopPre" ) );
        postWrap = Convert::String2T< TimelineWrapMethod >( deser.GetAttribute( "loopPost" ) );
    }
    else
    {
        preWrap = postWrap = Convert::String2T< TimelineWrapMethod >( loop );
    }

    if( !preWrap.IsValid() )
    {
        Warn< SerializationException >( deser, "Cannot desrialize timline's pre wrapping mode. Timeline's name: " + name + ". Replacing with CLAMP." );
        preWrap = TimelineWrapMethod::TWM_CLAMP;
    }

    if( !preWrap.IsValid() )
    {
        Warn< SerializationException >( deser, "Cannot desrialize timline's post wrapping mode. Timeline's name: " + name + ". Replacing with CLAMP." );
        postWrap = TimelineWrapMethod::TWM_CLAMP;
    }

    auto te = DefaultTimeline::Create( name, duration, preWrap, postWrap );

	auto local_time = deser.GetAttribute( "local_time" );
	if( local_time != "" )
	{
		te->SetLocalTime( Convert::String2T< float >( local_time, 0 ) );
	}

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
                else if( type == "trigger" )
                    te->AddKeyFrame( TimelineEventTrigger::Create( deser, te.get() ) );
                else
                    assert( false );

            } while( deser.NextChild() );

            deser.ExitChild(); // event
        }
        deser.ExitChild(); // events
    }

//    te->DeserializeChildren( deser );

    if( Convert::String2T< bool >( deser.GetAttribute( "play" ), false ) )
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


TimeType Direction2Factor( TimelinePlayDirection direction, bool paused )
{
    if( paused )
        return 0;
    else if( direction == TimelinePlayDirection::TPD_BACKWARD )
        return -1;
    else if( direction == TimelinePlayDirection::TPD_FORWAD )
        return 1;
    assert( false );
    return 0;
}

// *********************************
//
void                                DefaultTimeline::SetGlobalTimeImpl  ( TimeType t )
{
    if( m_triggeredPlay )
    {
        m_timeEvalImpl.UpdateGlobalTime( t );
        m_prevGlobalTime = t;
        m_timeEvalImpl.ResetLocalTimeTo( m_prevLocalTime );
        m_triggeredPlay = false;
        return;
    }

    auto globalTime = m_prevGlobalTime;
    m_prevGlobalTime = t;

    TimeType offset = t - globalTime; // FIXME: take direction intou account

    TimeType directionFactor = Direction2Factor( m_timeEvalImpl.GetDirection(), m_timeEvalImpl.IsPaused() );

    TimeType prevLocalTime = m_prevLocalTime;

    TimeType localTime = prevLocalTime;

    while( true )
    {
        auto event = CurrentEvent( localTime, localTime + directionFactor * offset ); // FIXME: direction should be taken into account
        if( event == nullptr || !event->IsActive() )
        {
            globalTime += offset; // move time to the end
            m_timeEvalImpl.UpdateGlobalTime( globalTime );
            break;
        }

        TimeType timeToEvent = abs( localTime - event->GetEventTime() ); // abs because time can go in both directions
        offset -= timeToEvent;

        globalTime += timeToEvent;
        m_timeEvalImpl.UpdateGlobalTime( globalTime );

        TriggerEventStep( event );
        localTime = m_timeEvalImpl.GetLocalTime();
    };

    m_prevLocalTime = m_timeEvalImpl.GetLocalTime();
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
    m_prevLocalTime = t;
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
    if( !m_timeEvalImpl.IsActive() || m_timeEvalImpl.IsPaused() )
        m_triggeredPlay = true;

    m_timeEvalImpl.Start();
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
    m_prevLocalTime = t;
    m_timeEvalImpl.ResetLocalTimeTo( t );

    PostUpdateEventStep();

    Stop();
}

// *********************************
//
void                                DefaultTimeline::SetTimeAndPlay     ( TimeType t )
{
    m_prevLocalTime = t;
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

    if( abs( m_prevLocalTime - evt->GetEventTime() ) < GEpsilon )
        TriggerEventStep( evt.get() );

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
    if( evn.get() == m_triggeredEvent ) 
        m_triggeredEvent = nullptr;

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
            if( evn.get() == m_triggeredEvent )
                m_triggeredEvent = nullptr;


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
    assert( m_triggeredEvent == evt || m_triggeredEvent == nullptr );

    if( m_triggeredEvent != evt )
    {
        //printf( "Deactivating %s \n", evt->GetName().c_str() );

        m_triggeredEvent = evt;
        m_triggeredEvent->SetActive( false ); //FIXME: not necessarily useful
    }
}

// *********************************
//
ITimelineEvent *                    DefaultTimeline::CurrentEvent       ( TimeType curTime, TimeType nextTime ) const
{
    if( m_timeEvalImpl.IsActive() && !m_timeEvalImpl.IsPaused() )
    {        
        auto t0 = std::min( curTime, nextTime ) - GEpsilon;
        auto t1 = std::max( curTime, nextTime ) + GEpsilon;

        bool reverseDirection = curTime > nextTime;

        if( reverseDirection )
        {
            for( int i = (int)m_keyFrameEvents.size() - 1; i >= 0; --i  )
            {
                auto & evt = m_keyFrameEvents[ i ];
                auto eventTime = evt->GetEventTime();

                if( abs( eventTime - curTime ) < GEpsilon )
                    continue;

                if( eventTime >= t0 && eventTime <= t1 )
                {
                    return evt.get();
                }
            }
        }
        else
        {
            for( auto evt : m_keyFrameEvents )
            {
                auto eventTime = evt->GetEventTime();

                if( abs( eventTime - curTime ) < GEpsilon )
                    continue;

                if( eventTime >= t0 && eventTime <= t1 )
                {
                    return evt.get();
                }
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
void                                DefaultTimeline::TriggerEventStep( ITimelineEvent * evt )
{
    auto curTime = m_prevLocalTime;
    auto prevTime = m_prevLocalTime;

    TimelineKeyframeEvent::KeyframeType keyframeType = TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail;
    switch( evt->GetType() )
    {
        case TimelineEventType::TET_STOP:
        {
            SetLocalTime( evt->GetEventTime() );
            m_timeEvalImpl.Stop();

            LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] stoppped on event [" << evt->GetName() << "] at time [" << curTime << "], (previous time [" << prevTime << "]) event time [" << evt->GetEventTime() << "]";

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

                    m_prevLocalTime = evtImpl->GetTargetTime();

                    { auto event = CurrentEvent( m_prevLocalTime - GEpsilon, m_prevLocalTime + GEpsilon );
                    if( event ) TriggerEventStep( event ); }

                    keyframeType = TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe;

                    LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] goto keyframe [" << evt->GetName() << "] jumped to [" << evtImpl->GetTargetTime() << "] at time [" << curTime << "], event time [" << evt->GetEventTime() << "]";
                    break;
                case LoopEventAction::LEA_RESTART:
                    m_timeEvalImpl.Reset();

                    m_prevLocalTime = TimeType( 0.0 );

                    Play(); //FIXME: really start or should we wait for the user to trigger this timeline?

                    keyframeType = TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe;

                    LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] restart keyframe [" << evt->GetName() << "] at time [" << curTime << "], event time [" << evt->GetEventTime() << "]";
                    break;
                case LoopEventAction::LEA_REVERSE:
                    Reverse();

                    keyframeType = TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe;

                    LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] reverse keyframe [" << evt->GetName() << "] at time [" << curTime << "], event time [" << evt->GetEventTime() << "]";

                    break;
                default:
                    assert( false );
            }

            evtImpl->IncLoopCount();

            break;
        }
        case TimelineEventType::TET_TRIGGER:
        {
            auto evtImpl = static_cast< TimelineEventTrigger * >( evt );
//            evtImpl->SetActive( false );
            m_triggeredEvent = nullptr;

            evtImpl->SendEvents();

            keyframeType = TimelineKeyframeEvent::KeyframeType::TriggerEventKeyframe;

            LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] trigger keyframe [" << evt->GetName() << "] at time [" << curTime << "], event time [" << evt->GetEventTime() << "]";
            break;
        }
        case TimelineEventType::TET_NULL:
        {
            keyframeType = TimelineKeyframeEvent::KeyframeType::NullKeyframe;

            LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] null keyframe [" << evt->GetName() << "] at time [" << curTime << "], event time [" << evt->GetEventTime() << "]";
            break;
        }
    default:
        assert( false );
    }

    SimpleJsonSerializeObject ser;
    ser.SetAttribute( "TriggerEvent", "KeyframeTrigger" );
    ser.SetAttribute( "cmd", "KeyframeEvent" );
    ser.SetAttribute( "KeyframeType", Convert::T2String( keyframeType ) );
    ser.SetAttribute( "KeyframeName", evt->GetName() );
    ser.SetAttribute( "Timeline", this->GetName() );
    ser.SetAttribute( "SceneName", TimelineHelper::GetSceneName( this ) );
    ser.SetAttribute( "Time", Convert::T2String( this->GetLocalTime() ) );

    ResponseEventPtr msg = std::make_shared<ResponseEvent>();
    msg->Response = ser.GetString();
    msg->SocketID = SEND_BROADCAST_EVENT;
    GetDefaultEventManager().QueueResponse( msg );
}

// *********************************
//
void                                DefaultTimeline::PostUpdateEventStep    ()
{
    if( m_triggeredEvent && !m_triggeredEvent->IsActive() )
    {
        auto t = GetLocalTime();
     
        // We can null m_triggeredEvent when current time will pass 0.5 * GEvtTimeSeparation.
        // But we must avoid situation, that the same event will be selected again. This may happen
        // for example when m_prevTime i still before event trigger time.
        if( std::abs( t - m_triggeredEvent->GetEventTime() ) > GEvtTimeSeparation * 0.5f &&
            CurrentEvent( t, m_prevLocalTime ) != m_triggeredEvent )
        {
            LOG_MESSAGE( SeverityLevel::debug ) << "Timeline [" << GetName() << "] activating event: [" << m_triggeredEvent->GetName() << "], timeline time: [" << t << "], event time: [" << m_triggeredEvent->GetEventTime() << "]";

            m_triggeredEvent->SetActive( true );
            m_triggeredEvent = nullptr;
        }
    }
}

// ***********************
//
const std::string&        DefaultTimeline::GetType             () const
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
