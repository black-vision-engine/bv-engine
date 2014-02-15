namespace bv { namespace model {

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
SimpleParameterImpl< InterpolatorType, ValueType, type >::SimpleParameterImpl( const std::string & name, const InterpolatorType & interpolator, const ITimeEvaluator * evaluator )
    : AbstractModelParameter( name, type, evaluator )
    , m_interpolator( interpolator )
{
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
SimpleParameterImpl< InterpolatorType, ValueType, type >::~SimpleParameterImpl    ()
{
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline ValueType SimpleParameterImpl< InterpolatorType, ValueType, type >::Evaluate ( TimeType t ) const
{
    t = AbstractModelParameter::GetLocalEvaluationTime( t );
    return m_interpolator.Evaluate( t );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline  void        SimpleParameterImpl< InterpolatorType, ValueType, type >::SetVal    ( const ValueType & val, TimeType t )
{
    m_interpolator.AddKey( t, val );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
void *      SimpleParameterImpl< InterpolatorType, ValueType, type >::QueryParamTyped  ()
{
    return static_cast< void * >( this );
}

// *******************************
//
inline  ParamMat2::ParamMat2( const std::string & name, const Vec4Interpolator & transform, const ITimeEvaluator * evaluator )
    : Parent( name, transform, evaluator )
{
}

// *******************************
//
inline  glm::mat2   ParamMat2::Evaluate    ( TimeType t ) const
{
    glm::vec4 v = Parent::Evaluate( t );

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

// *******************************
//
inline  void        ParamMat2::SetVal    ( const glm::mat2 & val, TimeType t )
{
    m_interpolator.AddKey( t, glm::vec4( val[ 0 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 1 ] ) );
}

} //model
} //bv
