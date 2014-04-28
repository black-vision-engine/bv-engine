#include "TimeSegmentEvalImpl.h"

#include <cassert>


namespace bv { namespace model {

// *********************************
//
TimeSegmentEvalImpl::TimeSegmentEvalImpl ( TimeType duration, TimelinePlayDirection direction, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
    : m_duration( duration )
    , m_backwardStartTime( TimeType( 0.0 ) )
    , m_started( false )
    , m_paused( false )
    , m_pauseDuration( TimeType( 0.0 ) )
    , m_playDirection( direction )
    , m_globalTime( TimeType( 0.0 ) )
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
    }

    if( m_paused )
    {
        m_paused = false;
    }
    else
    {
        ResetLocalTimeTo( TimeType( 0.0 ) );
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
void        TimeSegmentEvalImpl::UpdateGlobalTime   ( TimeType t )
{
    auto prevGlobalTime = m_globalTime;
    m_globalTime = t;

    if ( !m_started )
    {
        m_startTime = t;
    }

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
    auto t = GetLocalTimeNoClamp();
    return EvalClamp( t );
}

// *********************************
//
void        TimeSegmentEvalImpl::SetWrapBehaviorPre  ( TimelineWrapMethod method )
{
    SetWrapEvaluatorPre( method );
}

// *********************************
//
void        TimeSegmentEvalImpl::SetWrapBehaviorPost ( TimelineWrapMethod method )
{
    SetWrapEvaluatorPost( method );
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
