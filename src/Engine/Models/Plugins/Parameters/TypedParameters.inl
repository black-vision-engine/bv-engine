namespace bv { namespace model {

// *******************************
//
template< typename InterpolatorType, typename ValueType >
inline ValueType ParamImpl< InterpolatorType, ValueType >::Evaluate ( TimeType t ) const
{
    t = BaseParameter::GetEvaluationTime( t );
    BaseParameter::SetLastEvaluatedTime( t );
    return m_interpolator.Evaluate( t );
}

// *******************************
//
inline TransformF & ParamTransform::TransformRef()
{
    return m_interpolator;
}

// *******************************
//
inline  glm::mat2   ParamMat2::Evaluate    ( TimeType t ) const
{
    glm::vec4 v = Parent::Evaluate( t );

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

} //model
} //bv
