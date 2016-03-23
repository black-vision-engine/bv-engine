namespace bv { namespace model {

// *******************************
//
inline ITimeEvaluatorPtr   AbstractModelParameter::GetTimeEvaluator    ()
{
    return m_timeEvaluator;
}

// *******************************
//
inline void AbstractModelParameter::SetTimeEvaluator                   ( ITimeEvaluatorPtr timeEvaluator )
{
    assert( timeEvaluator );
    m_timeEvaluator = timeEvaluator;
}

// *******************************
//
inline TimeType  AbstractModelParameter::GetLocalEvaluationTime        () const
{
    assert( m_timeEvaluator );
    return m_timeEvaluator->GetLocalTime();
}

} //model
} //bv
