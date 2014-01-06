#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Interfaces/ITimeEvaluator.h"

#include "Engine/Types/Enums.h"

#include "Engine/Models/Plugins/Parameters/BaseParameter.h"


namespace bv { namespace model {

// TODO: Move to another file
class BaseValue : public IValue
{
private:

    std::string             m_name;

protected:

    explicit BaseValue( const std::string & name )
        : m_name( name )
    {
    }

    virtual ~BaseValue(){}

public:

    virtual const std::string &     GetName ()  const { return m_name; }

};

class ParamFloat : public BaseParameter
{
private:

    FloatInterpolator   m_value;

public:

    virtual ParamType   GetParamType     ()          const { return ParamType::PT_FLOAT; }

    float               Evaluate         ( TimeType t )   const;

    explicit ParamFloat( const std::string & name, const FloatInterpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }

};

class ParamVec4 : public BaseParameter
{
private:

    Vec4Interpolator    m_value;

public:

    virtual ParamType   GetParamType     ()          const { return ParamType::PT_FLOAT4; }

    glm::vec4           Evaluate        ( TimeType t )   const;

    explicit ParamVec4( const std::string & name, const Vec4Interpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }
};

class ParamVec3 : public BaseParameter
{
private:

    Vec3Interpolator    m_value;

public:

    virtual ParamType   GetParamType     ()          const { return ParamType::PT_FLOAT3; }

    glm::vec3           Evaluate        ( TimeType t )   const;

    explicit ParamVec3( const std::string & name, const Vec3Interpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }
};

class ParamMat2 : public BaseParameter
{
private:

    Vec4Interpolator    m_value;

public:

    virtual ParamType   GetParamType    ()          const { return ParamType::PT_MAT2; }

    glm::mat2           Evaluate        ( TimeType t )  const;

    explicit ParamMat2( const std::string & name, const Vec4Interpolator & value, const ITimeEvaluator * evaluator = nullptr )
       : BaseParameter( name, evaluator )
       , m_value( value )
    {
    }

};


class ParamTransform : public BaseParameter
{
private:

    TransformF          m_value;

public:

    virtual ParamType   GetParamType        ()                  const { return ParamType::PT_MAT4; }

    glm::mat4           Evaluate            ( TimeType t )      const 
    { 
        t = BaseParameter::GetEvaluationTime( t );
        SetLastEvaluatedTime( t );
        return m_value.Evaluate( t );
    }

    explicit ParamTransform( const std::string & name, const TransformF & value, const ITimeEvaluator * evaluator = nullptr )
        : BaseParameter( name, evaluator )
        , m_value( value )
    {
    }

    TransformF &        TransformRef    ()
    {
        return m_value;
    }
};


class ValueFloat : public BaseValue
{
private:

    float                   m_value;

public:
    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    float                       GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const float & v )     { m_value = v; }

    explicit                    ValueFloat      ( const std::string & name );

    virtual ~ValueFloat(){}

};


class ValueVec4 : public BaseValue
{
private:

    glm::vec4                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT4; }
    virtual const char *        GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    const glm::vec4 &           GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const glm::vec4 & v ) { m_value = v; }

    explicit                    ValueVec4       ( const std::string & name );

    virtual ~ValueVec4(){}

};

class ValueMat2 : public BaseValue
{
private:

    glm::mat2                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT2; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    const glm::mat2&            GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const glm::mat2 & v ) { m_value = v; }

    explicit                    ValueMat2       ( const std::string & name );

    virtual ~ValueMat2(){}

};

class ValueMat4 : public BaseValue
{
private:

    glm::mat4                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT4; }
    virtual const char *        GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    const glm::mat4 &           GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const glm::mat4 & v ) { m_value = v; }

    explicit                    ValueMat4       ( const std::string & name );

    virtual ~ValueMat4(){}

};

} // model
} // bv
