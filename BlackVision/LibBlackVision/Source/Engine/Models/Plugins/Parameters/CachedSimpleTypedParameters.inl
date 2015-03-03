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

} }