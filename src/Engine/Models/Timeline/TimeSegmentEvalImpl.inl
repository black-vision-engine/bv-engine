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

    printf( "%f %f %f\n", m_startTime, m_backwardStartTime, m_pauseDuration );
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
    TimeType q = m_duration;
    TimeType r = divmod( t, &q );

    return r;
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalMirror          ( TimeType t ) const
{
    TimeType q = m_duration;
    TimeType r = divmod( t, &q );

    printf( " r = %.1f", r );
    //printf( " t = %.2f, q = %.2f r = %.1f, r(q) == %d", t, q, r, round( q ) );

    if( round( q ) % 2 == 0 )
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
    printf( " pre" );
    return m_wrapEvaluatorPre( t );
}

// *******************************
//
inline TimeType TimeSegmentEvalImpl::EvalPost            ( TimeType t ) const
{
    printf( " post" );
    return m_wrapEvaluatorPost( t );
}

} //model
} //bv
