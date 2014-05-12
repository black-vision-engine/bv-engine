namespace bv { namespace model {

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
SimpleParameterImpl< InterpolatorType, ValueType, type >::SimpleParameterImpl( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator )
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
inline ValueType SimpleParameterImpl< InterpolatorType, ValueType, type >::Evaluate () const
{
    auto t = GetLocalEvaluationTime();

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
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline ModelParamType  SimpleParameterImpl< InterpolatorType, ValueType, type >::Type   ()
{
    return type;
}


// *******************************
//
inline  ParamMat2::ParamMat2( const std::string & name, const Vec4Interpolator & transform, ITimeEvaluatorPtr evaluator )
    : Parent( name, transform, evaluator )
{
}

// *******************************
//
inline  glm::mat2   ParamMat2::Evaluate    () const
{
    glm::vec4 v = Parent::Evaluate();

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

// *******************************
//
inline  void        ParamMat2::SetVal    ( const glm::mat2 & val, TimeType t )
{
    m_interpolator.AddKey( t, glm::vec4( val[ 0 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 1 ] ) );
}

// *******************************
//
inline ModelParamType  ParamMat2::Type     ()
{
    return ModelParamType::MPT_MAT2;
}

} //model
} //bv
