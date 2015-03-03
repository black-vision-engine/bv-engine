namespace bv { namespace model {

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline ValueType CachedSimpleTypedParameters< InterpolatorType, ValueType, type >::Evaluate () const
{
    auto val = ParentImpl::Evaluate();

    prevValue = curValue;
    curValue = val;

    changed = curValue != prevValue; // FIXME: is this the semantic we want?
    
    return val;
}

// *******************************
//
template<class T>
inline  ParamEnum<T>::ParamEnum( const std::string & name, const IntInterpolator & transform, ITimeEvaluatorPtr evaluator )
    //: SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >( name, transform, evaluator )
    : ParentImpl( name, transform, evaluator )
{
}

// *******************************
//
template<class T>
inline  T   ParamEnum<T>::Evaluate    () const
{
    int v = SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >::Evaluate();

    return T(v);
}

// *******************************
//
template<class T>
inline  void        ParamEnum<T>::SetVal    ( const T & val, TimeType t )
{
    m_interpolator.AddKey( t, int(val) );
}

// *******************************
//
template<class T>
inline ModelParamType  ParamEnum<T>::Type     ()
{
    return ModelParamType::MPT_ENUM;
}

} }