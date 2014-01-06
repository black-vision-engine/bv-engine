namespace bv { namespace model {


// *******************************
//
inline  float       ParamFloat::Evaluate        ( TimeType t )  const
{
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.evaluate( t );
}

// *******************************
//
inline  glm::vec4   ParamVec4::Evaluate         ( TimeType t )  const
{
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.evaluate( t );
}

// *******************************
//
inline  glm::vec3   ParamVec3::Evaluate         ( TimeType t )  const
{
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.evaluate( t );
}

// *******************************
//
inline  glm::mat2   ParamMat2::Evaluate         ( TimeType t )  const
{
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    glm::vec4 v = m_value.evaluate( t );

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

// *******************************
//
inline TransformF & ParamTransform::TransformRef()
{
    return m_value;
}

// *******************************
//
inline glm::mat4    ParamTransform::Evaluate    ( TimeType t )  const 
{ 
    t = BaseParameter::GetEvaluationTime( t );
    SetLastEvaluatedTime( t );
    return m_value.Evaluate( t );
}

} //model
} //bv
