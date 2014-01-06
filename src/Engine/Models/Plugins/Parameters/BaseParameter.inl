namespace bv { namespace model {

// *******************************
//
inline void         BaseParameter::SetLastEvaluatedTime ( TimeType t ) const
{
    m_lastEvaluatedTime = t;
}

// *******************************
//
inline TimeType     BaseParameter::GetLastEvaluatedTime () const
{
    return m_lastEvaluatedTime;
}

// *******************************
//
inline std::string  BaseParameter::GetName              ()  const 
{ 
    return m_name; 
}

// *******************************
//
inline void     BaseParameter::SetTimeEvaluator         ( const ITimeEvaluator * timeEvaluator )
{
    assert( timeEvaluator );

    m_timeEvaluator = timeEvaluator;
}

// *******************************
//
inline bool     BaseParameter::IsEvaluationNeeded       ( TimeType t )  const
{
    return t != m_lastEvaluatedTime;
}

// *********************************
//
inline TimeType BaseParameter::GetEvaluationTime        ( TimeType t )  const
{
    return m_timeEvaluator->Evaluate( t );
}

} //model
} //bv
