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
void SimpleParameterImpl< InterpolatorType, ValueType, type >::SetCurveType    ( CurveType type )
{ 
    m_interpolator.SetCurveType( type ); 
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
InterpolatorType &		SimpleParameterImpl< InterpolatorType, ValueType, type >::AccessInterpolator	()
{
	return m_interpolator;
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
VoidPtr     SimpleParameterImpl< InterpolatorType, ValueType, type >::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline ModelParamType  SimpleParameterImpl< InterpolatorType, ValueType, type >::Type   ()
{
    return type;
}

//// *******************************
////
//template< typename InterpolatorType, typename ValueType, ModelParamType type >
//void                SimpleParameterImpl< InterpolatorType, ValueType, type >::SetInterpolationMethod ( IParameter::InterpolationMethod method )
//{
//    __super::SetInterpolationMethod( method );
//    m_interpolator.SetInterpolationMethod( method );
//}
//
//// *******************************
////
//template< typename InterpolatorType, typename ValueType, ModelParamType type >
//IParameter::InterpolationMethod SimpleParameterImpl< InterpolatorType, ValueType, type >::GetInterpolationMethod () const
//{
//    assert( __super::GetInterpolationMethod() == m_interpolator.GetInterpolationMethod() ); // just to make sure we're consistent
//    return __super::GetInterpolationMethod();
//}


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

//// *******************************
////
//template<class T>
//inline  ParamEnum<T>::ParamEnum( const std::string & name, const IntInterpolator & transform, ITimeEvaluatorPtr evaluator )
//    : SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >( name, transform, evaluator )
//{
//}
//
//// *******************************
////
//template<class T>
//inline  T   ParamEnum<T>::Evaluate    () const
//{
//    int v = SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >::Evaluate();
//
//    return T(v);
//}
//
//// *******************************
////
//template<class T>
//inline  void        ParamEnum<T>::SetVal    ( const T & val, TimeType t )
//{
//    m_interpolator.AddKey( t, int(val) );
//}
//
//// *******************************
////
//template<class T>
//inline ModelParamType  ParamEnum<T>::Type     ()
//{
//    return ModelParamType::MPT_ENUM;
//}

} //model
} //bv
