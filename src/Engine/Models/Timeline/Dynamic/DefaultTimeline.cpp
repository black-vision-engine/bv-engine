#include "DefaultTimeline.h"

#include <cassert>
#include <algorithm>

#include "Engine/Models/Interfaces/ITimelineEvent.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"


namespace bv { namespace model {

namespace {

const TimeType GEvtTimeSeparation = TimeType( 0.1 );

// *********************************
//
bool timelineEventComparator( ITimelineEvent * e0, ITimelineEvent * e1 )
{
    return e0->GetLastTriggerTime() < e1->GetLastTriggerTime();
}

} //anonymous


// *********************************
//
DefaultTimeline::DefaultTimeline     ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod, ITimeEvaluator * parent )
    : m_timeEvalImpl( duration, TimelinePlayDirection::TPD_FORWAD, preMethod, postMethod )
    , m_prevTime( TimeType( 0.0 ) )
    , m_name( name )
    , m_parent( parent )
{
}

// *********************************
//
DefaultTimeline::~DefaultTimeline    ()
{
    for( auto evt : m_keyFrameEvents )
    {
        delete evt;
    }
}

// *********************************
//
const std::string &                 DefaultTimeline::GetName             () const
{
    return m_name;
}

// *********************************
//
TimeType                            DefaultTimeline::Evaluate            ( TimeType t ) const
{
    return m_timeEvalImpl.GlobalToLocal( t );
}

// *********************************
//
std::vector< IParameter * > &       DefaultTimeline::GetParameters       ()
{
    return m_registeredParameters;
}

// *********************************
//
IParameter *                        DefaultTimeline::GetParameter        ( const std::string & name )
{
    for( auto p : m_registeredParameters )
    {
        if( p->GetName() == name )
        {
            return p;
        }
    }

    return nullptr;
}

// *********************************
//
void                                DefaultTimeline::Update              ( TimeType t )
{
    if( m_parent )
    {
        t = m_parent->Evaluate( t );
    }

    m_timeEvalImpl.UpdateGlobalTime( t );
    auto tOld = m_timeEvalImpl.GetLocalTime();

    auto evt = CurrentEventNC();

    if( evt != nullptr )
    {
        TriggerEvent( evt, t );
    }

    m_prevTime = tOld;
}

// *********************************
//
TimeType                            DefaultTimeline::GetDuration         () const
{
    return m_timeEvalImpl.GetDuration();
}

// *********************************
//
void                                DefaultTimeline::Restart             ()
{
    m_timeEvalImpl.Reset();
}

// *********************************
//
void                                DefaultTimeline::SetPlayDirection    ( TimelinePlayDirection direction )
{
    if( m_timeEvalImpl.GetDirection() != direction )
    {
        m_timeEvalImpl.Reverse();
    }
}

// *********************************
//
TimelinePlayDirection               DefaultTimeline::GetPlayDirection    () const
{
    return m_timeEvalImpl.GetDirection();
}

// *********************************
//
void                                DefaultTimeline::Reverse             ()
{
    m_timeEvalImpl.Reverse();
}

// *********************************
//
void                                DefaultTimeline::Play                ()
{
    m_timeEvalImpl.Start();
}

// *********************************
//
void                                DefaultTimeline::Stop                ()
{
    m_timeEvalImpl.Stop();
}

// *********************************
//
void                                DefaultTimeline::SetTimeAndStop      ( TimeType t )
{
    m_timeEvalImpl.Stop();
    m_timeEvalImpl.ResetLocalTimeTo( t );
}

// *********************************
//
void                                DefaultTimeline::SetTimeAndPlay      ( TimeType t )
{
    m_timeEvalImpl.ResetLocalTimeTo( t );
    m_timeEvalImpl.Start();
}

// *********************************
//
void                                DefaultTimeline::SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    m_timeEvalImpl.SetWrapBehavior( preMethod, postMethod );
}

// *********************************
//
TimelineWrapMethod                  DefaultTimeline::GetWrapBehaviorPre  () const
{
    return m_timeEvalImpl.GetWrapPre();
}

// *********************************
//
TimelineWrapMethod                  DefaultTimeline::GetWrapBehaviorPost () const
{
    return m_timeEvalImpl.GetWrapPost();
}

// *********************************
//
void                                DefaultTimeline::SetLocalTime        ( TimeType t )
{
    m_timeEvalImpl.ResetLocalTimeTo( t );
}

// *********************************
//
TimeType                            DefaultTimeline::GetLocalTime        () const
{
    return m_timeEvalImpl.GetLocalTime();
}

// *********************************
//
unsigned int                        DefaultTimeline::NumKeyFrames        () const
{
    return m_keyFrameEvents.size();
}

// *********************************
//
bool                                DefaultTimeline::AddKeyFrame         ( ITimelineEvent * evt )
{
    if( !CanBeInserted( evt ) )
    {
        return false;
    }

    m_keyFrameEvents.push_back( evt );

    std::sort( m_keyFrameEvents.begin(), m_keyFrameEvents.end(), timelineEventComparator );

    return true;
}

// *********************************
//
const ITimelineEvent *              DefaultTimeline::GetKeyFrameEvent    ( const std::string & name ) const
{
    for( auto e : m_keyFrameEvents )
    {
        if ( e->GetName() == name )
        {
            return e;
        }
    }

    return nullptr;
}

// *********************************
//
const ITimelineEvent *              DefaultTimeline::GetKeyFrameEvent    ( unsigned int idx ) const
{
    if( idx >= m_keyFrameEvents.size() )
    {
        return nullptr;
    }

    return m_keyFrameEvents[ idx ];
}

// *********************************
//
bool                                DefaultTimeline::RemoveKeyFrameEvent ( unsigned int idx )
{
    assert( idx < m_keyFrameEvents.size() );

    m_keyFrameEvents.erase( m_keyFrameEvents.begin() + idx );

    return true;
}

// *********************************
//
bool                                DefaultTimeline::RemoveKeyFrameEvent ( const std::string & name )
{
    for( auto it = m_keyFrameEvents.begin(); it != m_keyFrameEvents.end(); ++it )
    {
        if ( (*it)->GetName() == name )
        {
            m_keyFrameEvents.erase( it );

            return true;
        }
    }

    return false;
}

// *********************************
//
const ITimelineEvent *              DefaultTimeline::CurrentEvent        () const
{
    return CurrentEventNC();
}

// *********************************
//
const ITimelineEvent *              DefaultTimeline::LastTriggeredEvent  () const
{
    if( CurrentEventNC() == m_lastTriggeredEvent )
    {
        return m_lastTriggeredEvent;
    }

    return nullptr;
}

// *********************************
//
bool                                DefaultTimeline::AddParameter        ( IParameter * param )
{
    if( std::find( m_registeredParameters.begin(), m_registeredParameters.end(), param ) == m_registeredParameters.end() )
    {
        m_registeredParameters.push_back( param );
        
        return true;
    }

    return false;
}

// *********************************
//
bool                                DefaultTimeline::RemoveParameter     ( const IParameter * param )
{
    auto it = std::find( m_registeredParameters.begin(), m_registeredParameters.end(), param );

    if( it != m_registeredParameters.end() )
    {
        m_registeredParameters.erase( it );

        return true;
    }

    return false;    
}

// *********************************
//
unsigned int                        DefaultTimeline::RemoveParameters     ( const std::string & name )
{
    unsigned int erasedElements = 0;

    for( auto it = m_registeredParameters.begin(); it != m_registeredParameters.end(); )
    {
        if( name == (*it)->GetName() )
        {
            it = m_registeredParameters.erase( it );

            ++erasedElements;
        }
        else
        {
            ++it;
        }
    }

    return erasedElements;
}

// *********************************
//
ITimelineEvent *                    DefaultTimeline::CurrentEventNC      () const
{
    if( m_timeEvalImpl.IsActive() )
    {
        auto t = m_timeEvalImpl.GetLocalTime();
        
        auto t0 = std::min( t, m_prevTime );
        auto t1 = std::max( t, m_prevTime );

        for( auto evt : m_keyFrameEvents )
        {
            auto eventTime = evt->GetEventTime();

            if( eventTime > t0 && eventTime <= t1 )
            {
                return evt;
            }
        }
    }

    return nullptr;
}

// *********************************
//
bool                                DefaultTimeline::CanBeInserted       ( const ITimelineEvent * evt ) const
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
void                                DefaultTimeline::TriggerEvent        ( ITimelineEvent * evt, TimeType globalTime )
{
    if( evt->IsActive() )
    {
        m_lastTriggeredEvent = evt;

        switch( evt->GetType() )
        {
            case TimelineEventType::TET_STOP:
            {
                auto evtImpl = static_cast< TimelineEventStop * >( evt );
                evtImpl->SetLastTriggerTime( globalTime );
                evtImpl->SetActive( false );
                m_timeEvalImpl.Stop();
                printf( "Event STOP\n" );

                break;
            }
            case TimelineEventType::TET_LOOP:
            {
                auto evtImpl = static_cast< TimelineEventLoop * >( evt );
                evtImpl->SetLastTriggerTime( globalTime );

                switch( evtImpl->GetActionType() )
                {
                    case LoopEventAction::LEA_GOTO:
                        m_timeEvalImpl.ResetLocalTimeTo( evtImpl->GetTargetTime() );
                        break;
                    case LoopEventAction::LEA_RESTART:
                        m_timeEvalImpl.Reset();
                        m_timeEvalImpl.Start(); //FIXME: really start or should we wait for the user to trigger this timeline?
                        break;
                    case LoopEventAction::LEA_REVERSE:
                        m_timeEvalImpl.Reverse();
                        break;
                    default:
                        assert( false );
                }

                evtImpl->IncLoopCount();
                
                printf( "Event LOOP %d\n", evtImpl->GetLoopCount() );

                break;
            }
            case TimelineEventType::TET_NULL:
            {
                auto evtImpl = static_cast< TimelineEventNull * >( evt );
                evtImpl->SetLastTriggerTime( globalTime );
                evtImpl->SetActive( false );

                printf( "Event NULL\n" );

                break;
            }
            default:
                assert( false );
        }
    }
}

} //model
} //bv
