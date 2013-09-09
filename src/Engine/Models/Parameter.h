#include "IParameter.h"
#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

namespace bv {

class BaseParameter : public IParameter
{
    mutable float   m_lastEvaluatedTime;

protected:

    void            SetLastEvaluatedTime(float t)   const;
    float           GetLastEvaluatedTime()          const;

    explicit BaseParameter(const std::string& name, ParameterSemantic semantic);
    virtual  ~BaseParameter(){}

public:
    bool            IsEvaluationNeeded(float t)     const;
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


class ParamTransform : public BaseParameter
{
private:
    TransformF          m_value;

public:
    virtual ParamType   GetParamType     ()          const { return ParamType::PT_MAT4; }

    glm::mat4           Evaluate        (float t)   const { SetLastEvaluatedTime(t); return m_value.evaluate(t); }

    explicit ParamTransform(const std::string& name, const TransformF& value, ParameterSemantic semantic = ParameterSemantic::NONE)
        : BaseParameter(name, semantic)
        , m_value(value)
    {
        m_semantic = semantic;
    }
};


class ValueFloat : public BaseValue
{
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
    glm::vec4                   m_value;
public:
    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT4; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>(&m_value); }

    const glm::vec4&            GetValue        ()              const   { return m_value; }
    void                        SetValue        (const glm::vec4& v)    { m_value = v; }

    explicit                    ValueVec4       (const std::string& name);

    virtual ~ValueVec4(){}
};

class ValueMat4 : public BaseValue
{
    glm::mat4                   m_value;
public:
    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT4; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>(&m_value); }

    const glm::mat4&            GetValue        ()              const   { return m_value; }
    void                        SetValue        (const glm::mat4& v)    { m_value = v; }

    explicit                    ValueMat4       (const std::string& name);

    virtual ~ValueMat4(){}
};

} // bv