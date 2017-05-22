namespace bv { namespace model {

// *******************************
//
inline void     TimeSegmentEvalImpl::ResetLocalTimeTo    ( TimeType t )
{
    m_pauseDuration = TimeType( 0.0 );
    m_startTime     = m_globalTime - t;

    if( m_playDirection == TimelinePlayDirection::TPD_BACKWARD )
    {
        m_backwardStartTime = m_globalTime;
    }
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::GetLocalTimeNoClamp () const
{
    return GetLocalTimeNoClamp( m_globalTime );
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::GetLocalTimeNoClamp ( TimeType globalTime ) const
{
    if( m_playDirection == TimelinePlayDirection::TPD_FORWAD )
    {
        auto t = globalTime - m_startTime - m_pauseDuration;

        return t;
    }
    else
    {
        return TimeType( 2.0 ) * m_backwardStartTime - m_startTime - globalTime + m_pauseDuration;
    }
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalClamp          ( TimeType t ) const
{
    if( t > m_duration )
    {
        return EvalPost( t );
    }
    else if( t < TimeType( 0.0 ) )
    {
        return EvalPre( t );
    }
    else
    {
        return t;
    }
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalPreClamp        ( TimeType t ) const
{
    { t; } // FIXME: suppress unused warning
    return TimeType( 0.0 );
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalPostClamp       ( TimeType t ) const
{
    { t; } // FIXME: suppress unused warning
    return m_duration;
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalRepeat          ( TimeType t ) const
{
    TimeType q = m_duration;
    TimeType r = mathematics::divmod( t, &q );

    return r;
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalMirror          ( TimeType t ) const
{
    TimeType q = m_duration;
    TimeType r = mathematics::divmod( t, &q );

    if( mathematics::round( q ) % 2 == 0 )
    {
        return r;
    }
    else
    {
        return m_duration - r;
    }
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalPre             ( TimeType t ) const
{
    return m_wrapEvaluatorPre( t );
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalPost            ( TimeType t ) const
{
    return m_wrapEvaluatorsPost[ (int) m_wrapPostBehavior ]( t );
    //return m_wrapEvaluatorPost( t );
}

} //model
} //bv
