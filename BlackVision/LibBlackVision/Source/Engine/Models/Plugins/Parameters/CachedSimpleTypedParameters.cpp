#include "CachedSimpleTypedParameters.h"

namespace bv { namespace model {

template< typename InterpolatorType, typename ValueType, ModelParamType type >
CachedSimpleTypedParameters< InterpolatorType, ValueType, type>::CachedSimpleTypedParameters( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator )
        : SimpleParameterImpl( name, interpolator, evaluator ),
        changed( true )
{ 
}


template< typename InterpolatorType, typename ValueType, ModelParamType type >
bool                    CachedSimpleTypedParameters< InterpolatorType, ValueType, type >::Changed() const
{
    Evaluate(); // FIXME: is this the semantic we want?
    return changed;
}

template class CachedSimpleTypedParameters< FloatInterpolator, float, ModelParamType::MPT_FLOAT >;
template class CachedSimpleTypedParameters< IntInterpolator, int, ModelParamType::MPT_INT >;
template class CachedSimpleTypedParameters< BoolInterpolator, bool, ModelParamType::MPT_BOOL >;
template class CachedSimpleTypedParameters< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >;
template class CachedSimpleTypedParameters< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >;
template class CachedSimpleTypedParameters< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2 >;

} }