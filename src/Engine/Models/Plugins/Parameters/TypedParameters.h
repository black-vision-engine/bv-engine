#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Parameters/BaseParameter.h"


namespace bv { namespace model {

// *******************************************
template< typename InterpolatorType, typename ValueType >
class ParamImpl : public BaseParameter
{
protected:

    InterpolatorType m_interpolator;

public:

    explicit            ParamImpl   ( const std::string & name, const InterpolatorType & interpolator, const ITimeEvaluator * evaluator = nullptr );
                        ~ParamImpl  ();

    inline  ValueType   Evaluate    ( TimeType t ) const;

};

// *******************************
//
template< typename InterpolatorType, typename ValueType >
ParamImpl< InterpolatorType, ValueType >::ParamImpl   ( const std::string & name, const InterpolatorType & interpolator, const ITimeEvaluator * evaluator )
    : BaseParameter( name, evaluator )
    , m_interpolator( interpolator )
{
}

// *******************************
//
template< typename InterpolatorType, typename ValueType >
ParamImpl< InterpolatorType, ValueType >::~ParamImpl  ()
{
}


// *******************************************
class ParamTransform : public ParamImpl< TransformF, glm::mat4 >
{
private:

    typedef ParamImpl< TransformF, glm::mat4 > Parent;

public:

    explicit    ParamTransform  ( const std::string & name, const TransformF & transform, const ITimeEvaluator * evaluator = nullptr )
        : Parent( name, transform, evaluator )
    {
    }

    inline TransformF & TransformRef();

};

// *******************************************
class ParamMat2 : public ParamImpl< Vec4Interpolator, glm::vec4 >
{
private:

    typedef ParamImpl< Vec4Interpolator, glm::vec4 > Parent;

public:

    explicit    ParamMat2   ( const std::string & name, const Vec4Interpolator & transform, const ITimeEvaluator * evaluator = nullptr )
        : Parent( name, transform, evaluator )
    {
    }

    inline  glm::mat2   Evaluate    ( TimeType t ) const;

};


typedef ParamImpl< FloatInterpolator, float >       ParamFloat;
typedef ParamImpl< Vec4Interpolator, glm::vec4 >    ParamVec4;
typedef ParamImpl< Vec3Interpolator, glm::vec3 >    ParamVec3;

} //model
} //bv

#include "TypedParameters.inl"
