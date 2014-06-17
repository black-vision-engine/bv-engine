#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"


namespace bv { namespace model {

// *******************************************
template< typename InterpolatorType, typename ValueType, ModelParamType type >
class SimpleParameterImpl : public AbstractModelParameter
{
public:

    typedef ValueType ValType;

protected:

    InterpolatorType m_interpolator;

public:

    explicit                SimpleParameterImpl ( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator );
                            ~SimpleParameterImpl();

    inline  ValueType       Evaluate            () const;
    inline  void            SetVal              ( const ValueType & val, TimeType t );

    virtual VoidPtr         QueryParamTyped     () override;

    inline static  ModelParamType  Type         ();

};


// *******************************************
class ParamMat2 : public SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 >
{
public:

    typedef glm::mat2 ValType;

private:

    typedef SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 > Parent;

public:

    inline explicit         ParamMat2       ( const std::string & name, const Vec4Interpolator & transform, ITimeEvaluatorPtr evaluator );

    inline  glm::mat2       Evaluate        () const;
    inline  void            SetVal          ( const glm::mat2 & val, TimeType t );

    virtual VoidPtr         QueryParamTyped () override;

    inline static  ModelParamType  Type     ();

};


typedef SimpleParameterImpl< FloatInterpolator, float, ModelParamType::MPT_FLOAT >      ParamFloat;
typedef SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >    ParamVec4;
typedef SimpleParameterImpl< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >    ParamVec3;

DEFINE_PTR_TYPE(ParamFloat)
DEFINE_PTR_TYPE(ParamVec4)
DEFINE_PTR_TYPE(ParamVec3)
DEFINE_PTR_TYPE(ParamMat2)


} //model
} //bv

#include "SimpleTypedParameters.inl"
