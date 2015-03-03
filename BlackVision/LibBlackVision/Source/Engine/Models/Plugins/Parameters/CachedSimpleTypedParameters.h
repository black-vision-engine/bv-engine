#pragma once

#include "Engine/Models/Plugins/Interfaces/ICachedParameter.h"
#include "SimpleTypedParameters.h"

namespace bv { namespace model {

// *******************************************
template< typename InterpolatorType, typename ValueType, ModelParamType type >
class CachedSimpleTypedParameters : 
    public SimpleParameterImpl< InterpolatorType, ValueType, type >
    , public ICachedParameter
{
    ValueType                       lastValue;
public:
    CachedSimpleTypedParameters( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator );

    virtual bool                    Changed() override;
};

typedef CachedSimpleTypedParameters< FloatInterpolator, float, ModelParamType::MPT_FLOAT >      ParamFloat;
typedef CachedSimpleTypedParameters< IntInterpolator, int, ModelParamType::MPT_INT >            ParamInt;
typedef CachedSimpleTypedParameters< BoolInterpolator, bool, ModelParamType::MPT_BOOL >         ParamBool;
typedef CachedSimpleTypedParameters< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >    ParamVec4;
typedef CachedSimpleTypedParameters< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >    ParamVec3;
typedef CachedSimpleTypedParameters< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2 >    ParamVec2;

//template<typename T>
//using ParamEnumPtr<T> = std::shared_ptr<T>;

DEFINE_PTR_TYPE(ParamBool)
DEFINE_PTR_TYPE(ParamInt)
DEFINE_PTR_TYPE(ParamFloat)
DEFINE_PTR_TYPE(ParamVec4)
DEFINE_PTR_TYPE(ParamVec3)
DEFINE_PTR_TYPE(ParamVec2)
DEFINE_PTR_TYPE(ParamMat2)

} //model
} //bv
