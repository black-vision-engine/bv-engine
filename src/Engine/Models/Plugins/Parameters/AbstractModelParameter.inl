namespace bv { namespace model {

// *******************************
//
inline void AbstractModelParameter::SetTimeEvaluator                ( const ITimeEvaluator * timeEvaluator )
{
    m_timeEvaluator = timeEvaluator;
}

// *******************************
//
inline  TimeType  AbstractModelParameter::GetLocalEvaluationTime    ( TimeType t ) const
{
    return m_timeEvaluator->Evaluate( t );
}

} //model
} //bv
