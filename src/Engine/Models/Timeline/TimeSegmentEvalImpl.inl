namespace bv { namespace model {

// *******************************
//
inline TimeType TimeSegmentEvalImpl::ResetLocalTimeTo    ( TimeType t )
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
    if( m_playDirection == TimelinePlayDirection::TPD_FORWAD )
    {
        return  m_globalTime - m_startTime - m_pauseDuration;
    }
    else
    {
        return TimeType( 2.0 ) * m_backwardStartTime - m_startTime - m_globalTime + m_pauseDuration;
    }
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::Clamp               ( TimeType t ) const
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
    return TimeType( 0.0 );
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalPostClamp       ( TimeType t ) const
{
    return m_duration;
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalRepeat          ( TimeType t ) const
{
    
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalMirror          ( TimeType t ) const
{
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
    return m_wrapEvaluatorPost( t );
}

} //model
} //bv
