#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"


namespace bv { namespace model {

// *******************************************
template< typename InterpolatorType, typename ValueType, ModelParamType type >
class SimpleParameterImpl : public AbstractModelParameter
{
protected:

    InterpolatorType m_interpolator;

public:

    explicit            SimpleParameterImpl ( const std::string & name, const InterpolatorType & interpolator, const ITimeEvaluator * evaluator = nullptr );
                        ~SimpleParameterImpl();

    inline  ValueType   Evaluate            ( TimeType t ) const;
    inline  void        SetVal              ( const ValueType & val, TimeType t );

    virtual void *      QueryParamTyped     () override;

};


// *******************************************
class ParamMat2 : public SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 >
{
private:

    typedef SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 > Parent;

public:

    inline explicit     ParamMat2       ( const std::string & name, const Vec4Interpolator & transform, const ITimeEvaluator * evaluator = nullptr );

    inline  glm::mat2   Evaluate        ( TimeType t ) const;
    inline  void        SetVal          ( const glm::mat2 & val, TimeType t );

    virtual void *      QueryParamTyped () override;

};


typedef SimpleParameterImpl< FloatInterpolator, float, ModelParamType::MPT_FLOAT >      ParamFloat;
typedef SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >    ParamVec4;
typedef SimpleParameterImpl< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >    ParamVec3;

} //model
} //bv

#include "SimpleTypedParameters.inl"
