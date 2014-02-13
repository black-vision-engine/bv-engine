#include "TimelineBase.h"

#include <cassert>


namespace bv { namespace model {

namespace {
  
    WrapMethod tob_2_wrap[] = { WrapMethod::clamp, WrapMethod::repeat, WrapMethod::pingPong };
}

// *********************************
//
TimelineBase::TimelineBase          ( TimeType startTime, TimeType endTime, const std::string & name, float scale )
    : m_name( name )
{
    m_settings.m_constValPre    = 0.f;
    m_settings.m_constValPost   = endTime - startTime;
    m_settings.m_postBehavior   = TimelineOutBehavior::TOB_CLAMP;
    m_settings.m_preBehavior    = TimelineOutBehavior::TOB_CLAMP;

    m_state.Reset();
 
    assert( startTime < endTime );
    
    m_interpolator.AddKey( startTime, 0.f );
    m_interpolator.AddKey( endTime, scale * ( endTime - startTime ) );
    m_interpolator.SetWrapMethod( WrapMethod::clamp, WrapMethod::clamp );
}

// *********************************
//
TimelineBase::TimelineBase          ( const TimeInterpolator & interpolator, const TimelineSettings & settings, const std::string & name )
    : m_settings( settings )
    , m_name( name )
{
    m_state.Reset();
}

// *********************************
//
TimelineBase::TimelineBase          ( const TimelineSettings & settings, TimeType startTime, TimeType endTime, const std::string & name, float scale )
    : m_settings( settings )
    , m_name( name )
{
    m_state.Reset();

    assert( startTime < endTime );

    m_interpolator.AddKey( startTime, 0.f );
    m_interpolator.AddKey( endTime, scale * ( endTime - startTime ) );
    m_interpolator.SetWrapMethod( WrapMethod::clamp, WrapMethod::clamp );

    if( m_settings.m_preBehavior != TimelineOutBehavior::TOB_CONST_VAL )
    {
        m_interpolator.SetWrapPreMethod( tob_2_wrap[ (int) m_settings.m_preBehavior ] );
    }

    if( m_settings.m_postBehavior != TimelineOutBehavior::TOB_CONST_VAL )
    {
        m_interpolator.SetWrapPostMethod( tob_2_wrap[ (int) m_settings.m_postBehavior ] );
    }
}

// *********************************
//
TimelineBase::~TimelineBase         ()
{
}

// *********************************
//
void   TimelineBase::SetInterval    ( TimeType startTime, TimeType endTime, float scale )
{
    assert( startTime < endTime );

    TimeInterpolator ti;

    ti.SetWrapMethod( m_interpolator.GetWrapPreMethod(), m_interpolator.GetWrapPostMethod() );
    ti.AddKey( startTime, 0.f );
    ti.AddKey( endTime, scale * ( endTime - startTime ) );

    m_interpolator = ti;
}

// *********************************
//
const std::string & TimelineBase::GetName () const
{
    return m_name;
}

// *********************************
//
TimeType    TimelineBase::Evaluate  ( TimeType t ) const
{
    if( t < StartTime() && m_settings.m_preBehavior == TimelineOutBehavior::TOB_CONST_VAL )
        return m_settings.m_constValPre;
    else if( t > EndTime() && m_settings.m_postBehavior == TimelineOutBehavior::TOB_CONST_VAL )
        return m_settings.m_constValPost;

    return m_interpolator.Evaluate( t );
}

// *********************************
//
TimeType  TimelineBase::StartTime       () const
{
    return m_interpolator.FirstKey().t;
}

// *********************************
//
TimeType  TimelineBase::EndTime         () const
{
    return m_interpolator.LastKey().t;
}

} //model
} //bv
