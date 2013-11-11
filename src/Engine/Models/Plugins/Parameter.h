#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

//FIXME: this shit deserves some templates :D
class BaseParameter : public IParameter
{
protected:
    std::string         m_name;
    ParameterSemantic   m_semantic;
    mutable float       m_lastEvaluatedTime;

protected:

    virtual std::string         GetName()       const { return m_name; }
    virtual ParameterSemantic   GetSemantic()   const { return m_semantic; }


    void                        SetLastEvaluatedTime(float t)   const;
    float                       GetLastEvaluatedTime()          const;

    explicit BaseParameter(const std::string& name, ParameterSemantic semantic);
    virtual  ~BaseParameter(){}

public:
    bool                        IsEvaluationNeeded(float t)     const;
};

// TODO: Move to another file
class BaseValue : public IValue
{
private:
    std::string             m_name;

protected:
    explicit BaseValue(const std::string& name)
        : m_name(name)
    {}

    virtual ~BaseValue(){}

public:
    virtual const std::string&  GetName     ()              const { return m_name; }
};

class ParamFloat : public BaseParameter
{
private:

    FloatInterpolator   m_value;

public:

    virtual ParamType   GetParamType     ()          const { return ParamType::PT_FLOAT; }

    float               Evaluate        (float t)   const;

    explicit ParamFloat(const std::string& name, const FloatInterpolator& value, ParameterSemantic semantic = ParameterSemantic::NONE)
       : BaseParameter(name, semantic)
       , m_value(value)
    {}
};

class ParamVec4 : public BaseParameter
{
private:
    Vec4Interpolator    m_value;

public:
    virtual ParamType   GetParamType     ()          const { return ParamType::PT_FLOAT4; }

    glm::vec4           Evaluate        (float t)   const;

    explicit ParamVec4(const std::string& name, const Vec4Interpolator& value, ParameterSemantic semantic = ParameterSemantic::NONE)
       : BaseParameter(name, semantic)
       , m_value(value)
    {}
};

class ParamMat2 : public BaseParameter
{
private:

    Vec4Interpolator    m_value;

public:

    virtual ParamType   GetParamType     ()          const { return ParamType::PT_MAT2; }

    glm::mat2           Evaluate        (float t)   const;

    explicit ParamMat2(const std::string& name, const Vec4Interpolator& value, ParameterSemantic semantic = ParameterSemantic::NONE)
       : BaseParameter(name, semantic)
       , m_value(value)
    {}
};


class ParamTransform : public BaseParameter
{
private:

    TransformF          m_value;

public:

    virtual ParamType   GetParamType     ()          const { return ParamType::PT_MAT4; }

    glm::mat4           Evaluate        (float t)   const { SetLastEvaluatedTime(t); return m_value.evaluate(t); }

    explicit ParamTransform(const std::string& name, const TransformF & value, ParameterSemantic semantic = ParameterSemantic::NONE)
        : BaseParameter(name, semantic)
        , m_value(value)
    {
        m_semantic = semantic;
    }
};


class ValueFloat : public BaseValue
{
private:

    float                   m_value;

public:
    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>(&m_value); }

    float                       GetValue        ()              const   { return m_value; }
    void                        SetValue        (const float& v)        { m_value = v; }

    explicit                    ValueFloat      (const std::string& name);

    virtual ~ValueFloat(){}
};


class ValueVec4 : public BaseValue
{
private:

    glm::vec4                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT4; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>(&m_value); }

    const glm::vec4&            GetValue        ()              const   { return m_value; }
    void                        SetValue        (const glm::vec4& v)    { m_value = v; }

    explicit                    ValueVec4       (const std::string& name);

    virtual ~ValueVec4(){}
};

class ValueMat2 : public BaseValue
{
private:

    glm::mat2                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT2; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>(&m_value); }

    const glm::mat2&            GetValue        ()              const   { return m_value; }
    void                        SetValue        (const glm::mat2 & v)   { m_value = v; }

    explicit                    ValueMat2       (const std::string & name);

    virtual ~ValueMat2(){}
};

class ValueMat4 : public BaseValue
{
private:

    glm::mat4                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT4; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>(&m_value); }

    const glm::mat4&            GetValue        ()              const   { return m_value; }
    void                        SetValue        (const glm::mat4& v)    { m_value = v; }

    explicit                    ValueMat4       (const std::string& name);

    virtual ~ValueMat4(){}
};

} // model
} // bv