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

const TimeType GEvtTimeSeparation = TimeType( 0.2 ); //FIXME: some config (default) parameters should be used here - engine should expose it's connfiguration defaults for overriding

// *********************************
//
bool timelineEventComparator    ( ITimelineEvent * e0, ITimelineEvent * e1 )
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
{
    for( auto evt : m_keyFrameEvents )
    {
        delete evt;
    }
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
bool                                DefaultTimeline::AddKeyFrame        ( ITimelineEvent * evt )
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
const ITimelineEvent *              DefaultTimeline::GetKeyFrameEvent   ( const std::string & name ) const
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
const ITimelineEvent *              DefaultTimeline::GetKeyFrameEvent   ( unsigned int idx ) const
{
    if( idx >= m_keyFrameEvents.size() )
    {
        return nullptr;
    }

    return m_keyFrameEvents[ idx ];
}

// *********************************
//
bool                                DefaultTimeline::RemoveKeyFrameEvent( unsigned int idx )
{
    assert( idx < m_keyFrameEvents.size() );

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
                return evt;
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

    switch( evt->GetType() )
    {
        case TimelineEventType::TET_STOP:
        {
            evt->SetActive( false );
            SetLocalTime( evt->GetEventTime() );
            m_timeEvalImpl.Stop();
            //printf( "Event STOP %s prev: %.4f, cur: %.4f\n", evt->GetName().c_str(), prevTime, curTime );
            printf( "Event STOP %s \n", evt->GetName().c_str() );
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

                    printf( "Event LOOP -> GOTO: %.4f %s\n", evtImpl->GetTargetTime(), evt->GetName().c_str() );
                    break;
                case LoopEventAction::LEA_RESTART:
                    m_timeEvalImpl.Reset();

                    m_prevTime = TimeType( 0.0 );

                    m_activeEvent->SetActive( true ); //Reset current event (forthcomming play will trigger its set of event problems - e.g. first event at 0.0 to pass by).
                    m_activeEvent = nullptr;

                    Play(); //FIXME: really start or should we wait for the user to trigger this timeline?
                    printf( "Event LOOP -> RESTART %s\n", evt->GetName().c_str() );                    
                    break;
                case LoopEventAction::LEA_REVERSE:
                    Reverse();
                    
                    printf( "Event LOOP -> REVERSE %s\n", evt->GetName().c_str() );
                    
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
            evtImpl->SetActive( false );

            printf( "Event NULL\n" );

            break;
        }
        default:
            assert( false );
    }
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

} //model
} //bv
