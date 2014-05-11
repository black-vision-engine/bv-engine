namespace bv { namespace model {

// *******************************
//
inline void AbstractModelParameter::SetTimeEvaluator                ( const ITimeEvaluator * timeEvaluator )
{
    m_timeEvaluator = timeEvaluator;
}

// *******************************
//
inline  TimeType  AbstractModelParameter::GetLocalEvaluationTime    () const
{
    assert( m_timeEvaluator );
    return m_timeEvaluator->GetLocalTime();
}

} //model
} //bv
