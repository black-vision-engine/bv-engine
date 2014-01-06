#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Parameters/BaseParameter.h"


namespace bv { namespace model {

// *******************************************
class ParamFloat : public BaseParameter
{
private:

    FloatInterpolator   m_value;

public:


    explicit ParamFloat( const std::string & name, const FloatInterpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }

    inline float        Evaluate    ( TimeType t )  const;

};

// *******************************************
class ParamVec4 : public BaseParameter
{
private:

    Vec4Interpolator    m_value;

public:

    explicit ParamVec4( const std::string & name, const Vec4Interpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }

    inline glm::vec4    Evaluate    ( TimeType t )  const;

};

// *******************************************
class ParamVec3 : public BaseParameter
{
private:

    Vec3Interpolator    m_value;

public:

    explicit ParamVec3( const std::string & name, const Vec3Interpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }

    inline glm::vec3    Evaluate    ( TimeType t )  const;

};

// *******************************************
class ParamMat2 : public BaseParameter
{
private:

    Vec4Interpolator    m_value;

public:

    explicit ParamMat2( const std::string & name, const Vec4Interpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }

    inline glm::mat2    Evaluate    ( TimeType t )  const;

};


class ParamTransform : public BaseParameter
{
private:

    TransformF          m_value;

public:

    explicit ParamTransform( const std::string & name, const TransformF & value, const ITimeEvaluator * evaluator = nullptr )
        : BaseParameter( name, evaluator )
        , m_value( value )
    {
    }

    inline TransformF & TransformRef();
    inline glm::mat4    Evaluate    ( TimeType t )  const;

};

} //model
} //bv

#include "TypedParameters.inl"
