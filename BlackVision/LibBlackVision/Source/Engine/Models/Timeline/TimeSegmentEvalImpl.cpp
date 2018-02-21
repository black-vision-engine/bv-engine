#include "stdafx.h"

#include "TimeSegmentEvalImpl.h"

#include <cassert>




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *********************************
//
TimeSegmentEvalImpl::TimeSegmentEvalImpl ( TimeType duration, TimelinePlayDirection direction, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
    : m_duration( duration )
    , m_backwardStartTime( TimeType( 0.0 ) )
    , m_started( false )
    , m_paused( false )
    , m_startTriggered( false )
    , m_pauseDuration( TimeType( 0.0 ) )
    , m_playDirection( direction )
    , m_globalTime( TimeType( 0.0 ) )
    , m_startTime( TimeType( 0.0 ) )
    , m_wrapPreBehavior( preMethod ) // FIXME just for serialization actually
    , m_wrapPostBehavior( postMethod ) // FIXME just for serialization actually
{
    assert( duration > TimeType( 0.0 ) );

    InitWrapEvaluators( preMethod, postMethod );
}

// *********************************
//
TimeSegmentEvalImpl::~TimeSegmentEvalImpl()
{
}
                
// *********************************
//
void        TimeSegmentEvalImpl::Start               ()
{
    if( !m_started )
    {
        m_started = true;
        m_startTriggered = true;
    }

    if( m_paused )
    {
        m_paused = false;
    }
    else
    {
        //FIXME: WHAT THE HELL IS THIS?????
        //ResetLocalTimeTo( TimeType( 0.0 ) );
    }
}

// *********************************
//
void        TimeSegmentEvalImpl::Stop                ()
{
    if ( m_started && !m_paused )
    {
        m_paused = true;
    }
}

// *********************************
//
void        TimeSegmentEvalImpl::Reverse             ()
{
    auto t = GetLocalTime();

    if ( m_playDirection == TimelinePlayDirection::TPD_FORWAD )
    {
        m_playDirection = TimelinePlayDirection::TPD_BACKWARD;
    }
    else
    {
        m_playDirection = TimelinePlayDirection::TPD_FORWAD;
    }

    ResetLocalTimeTo( t );
}

// *********************************
//
void        TimeSegmentEvalImpl::Reset               ( TimelinePlayDirection direction )
{
    m_started = false;
    m_paused = false;
    m_playDirection = direction;

    ResetLocalTimeTo( TimeType( 0.0 ) );
}

// *********************************
//
bool        TimeSegmentEvalImpl::IsActive           () const
{
    return m_started;
}

// *********************************
//
bool        TimeSegmentEvalImpl::IsPaused           () const
{
    return m_paused;
}

// *********************************
//
void        TimeSegmentEvalImpl::UpdateGlobalTime   ( TimeType t )
{
    auto prevGlobalTime = m_globalTime;
    m_globalTime = t;

    if ( !m_started )
    {
        //m_startTime = t;
        m_startTime += t - prevGlobalTime;
    }

    //if( m_startTriggered )
    //{
    //    ResetLocalTimeTo(  )
    //}

    //FIXME: this is not the best way to accumulate pause times but should suffice for the time being
    if( m_paused )
    {
        m_pauseDuration += t - prevGlobalTime;
    }

}

// *********************************
//
TimeType    TimeSegmentEvalImpl::GetLocalTime        () const
{
    auto t = GetLocalTimeNoClamp( m_globalTime );

    return EvalClamp( t );
}

// *********************************
//
TimeType    TimeSegmentEvalImpl::GlobalToLocal      ( TimeType t ) const
{
    auto tnc = GetLocalTimeNoClamp( t );
    return EvalClamp( tnc );
}

// *********************************
//
void        TimeSegmentEvalImpl::SetWrapBehaviorPre  ( TimelineWrapMethod method )
{
    SetWrapEvaluatorPre( method );
	m_wrapPreBehavior = method;
}

// *********************************
//
void        TimeSegmentEvalImpl::SetWrapBehaviorPost ( TimelineWrapMethod method )
{
    SetWrapEvaluatorPost( method );
	m_wrapPostBehavior = method;
}

// *********************************
//
void        TimeSegmentEvalImpl::SetWrapBehavior     ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    SetWrapBehaviorPre( preMethod );
    SetWrapBehaviorPost( postMethod );
}

// *********************************
//
TimelineWrapMethod      TimeSegmentEvalImpl::GetWrapPre          () const
{
    return m_wrapPreBehavior;
}

// *********************************
//
TimelineWrapMethod      TimeSegmentEvalImpl::GetWrapPost         () const
{
    return m_wrapPostBehavior;
}

// *********************************
//
void		TimeSegmentEvalImpl::SetDuration         ( TimeType duration )
{
    m_duration = duration;
}

// *********************************
//
TimeType    TimeSegmentEvalImpl::GetDuration         () const
{
    return m_duration;
}

// *********************************
//
TimelinePlayDirection   TimeSegmentEvalImpl::GetDirection        () const
{
    return m_playDirection;
}

// *********************************
//
void    TimeSegmentEvalImpl::InitWrapEvaluators ( TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    //@see: http://en.cppreference.com/w/cpp/utility/functional/placeholders
    using std::placeholders::_1;

    m_wrapEvaluatorsPre.push_back( std::bind( &TimeSegmentEvalImpl::EvalPreClamp, this, _1 ) );
    m_wrapEvaluatorsPre.push_back( std::bind( &TimeSegmentEvalImpl::EvalRepeat, this, _1 ) );
    m_wrapEvaluatorsPre.push_back( std::bind( &TimeSegmentEvalImpl::EvalMirror, this, _1 ) );

    m_wrapEvaluatorsPost.push_back( std::bind( &TimeSegmentEvalImpl::EvalPostClamp, this, _1 ) );
    m_wrapEvaluatorsPost.push_back( std::bind( &TimeSegmentEvalImpl::EvalRepeat, this, _1 ) );
    m_wrapEvaluatorsPost.push_back( std::bind( &TimeSegmentEvalImpl::EvalMirror, this, _1 ) );

    SetWrapEvaluatorPre( preMethod );
    SetWrapEvaluatorPost( postMethod );
}

// *********************************
//
void    TimeSegmentEvalImpl::SetWrapEvaluatorPre ( TimelineWrapMethod method )
{
    m_wrapEvaluatorPre = m_wrapEvaluatorsPre[ (int) method ];
}

// *********************************
//
void    TimeSegmentEvalImpl::SetWrapEvaluatorPost( TimelineWrapMethod method )
{
    m_wrapEvaluatorPost = m_wrapEvaluatorsPost[ (int) method ];
}

} //model
} //bv
