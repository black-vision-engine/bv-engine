#include "TimelineBase.h"

#include <cassert>


namespace bv { namespace model {

namespace {
  
    WrapMethod tob_2_wrap[] = { WrapMethod::clamp, WrapMethod::repeat, WrapMethod::pingPong };
}

// *********************************
//
TimelineBase::TimelineBase          ( TimeType startTime, TimeType endTime, float scale )
{
    m_settings.m_constValPre    = 0.f;
    m_settings.m_constValPost   = endTime - startTime;
    m_settings.m_postBehavior   = TimelineOutBehavior::TOB_CLAMP;
    m_settings.m_preBehavior    = TimelineOutBehavior::TOB_CLAMP;

    m_state.Reset();
 
    assert( startTime < endTime );
    
    m_interpolator.addKey( startTime, 0.f );
    m_interpolator.addKey( endTime, scale * ( endTime - startTime ) );
    m_interpolator.setWrapMethod( WrapMethod::clamp, WrapMethod::clamp );
}

// *********************************
//
TimelineBase::TimelineBase          ( const TimeInterpolator & interpolator, const TimelineSettings & settings )
    : m_settings( settings )
{
    m_state.Reset();
}

// *********************************
//
TimelineBase::TimelineBase          ( const TimelineSettings & settings, TimeType startTime, TimeType endTime, float scale )
    : m_settings( settings )
{
    m_state.Reset();

    assert( startTime < endTime );

    m_interpolator.addKey( startTime, 0.f );
    m_interpolator.addKey( endTime, scale * ( endTime - startTime ) );
    m_interpolator.setWrapMethod( WrapMethod::clamp, WrapMethod::clamp );

    if( m_settings.m_preBehavior != TimelineOutBehavior::TOB_CONST_VAL )
    {
        m_interpolator.setWrapPreMethod( tob_2_wrap[ (int) m_settings.m_preBehavior ] );
    }

    if( m_settings.m_postBehavior != TimelineOutBehavior::TOB_CONST_VAL )
    {
        m_interpolator.setWrapPostMethod( tob_2_wrap[ (int) m_settings.m_postBehavior ] );
    }
}

// *********************************
//
TimelineBase::~TimelineBase         ()
{
}

// *********************************
//
TimeType    TimelineBase::Evaluate  ( TimeType t ) const
{
    return 0.f;
    //if( t > m_endTime && m_settings.m_preBehavior == TimelineOutBehavior::TOB_CONST_VAL )
    //    return m_settings.m_constValPre;

    //    return EvalPre( t );
    //else if ( t < m_startTime )
    //    return EvalPost( t );

    //return t - m_startTime;

}

// *********************************
//
TimeType    TimelineBase::EvalPre   ( TimeType t ) const
{
    return 0.f;
}

// *********************************
//
TimeType    TimelineBase::EvalPost  ( TimeType t ) const
{
    return 0.f;
}

} //model
} //bv
