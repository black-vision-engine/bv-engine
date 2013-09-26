#include "Parameter.h"

namespace bv { namespace model {

void            BaseParameter::SetLastEvaluatedTime(float t) const
{
    m_lastEvaluatedTime = t;
}

float           BaseParameter::GetLastEvaluatedTime()          const
{
    return m_lastEvaluatedTime;
}

bool            BaseParameter::IsEvaluationNeeded(float t)     const
{
    return t != m_lastEvaluatedTime;
}

BaseParameter::BaseParameter(const std::string& name, ParameterSemantic semantic)
    : m_lastEvaluatedTime(-1.f) // TODO: Is it goot idea to set in to -1 ?
{
}

ValueFloat::ValueFloat(const std::string& name)
    : BaseValue(name)
{}

ValueVec4::ValueVec4(const std::string& name)
    : BaseValue(name)
{}

ValueMat4::ValueMat4(const std::string& name)
    : BaseValue(name)
{}

float       ParamFloat::Evaluate        (float t)   const
{
    SetLastEvaluatedTime(t);
    return m_value.evaluate(t);
}

glm::vec4       ParamVec4::Evaluate(float t)    const
{
    SetLastEvaluatedTime(t);
    return m_value.evaluate(t);
}

glm::mat2           ParamMat2::Evaluate        (float t)   const
{
    SetLastEvaluatedTime(t);
    glm::vec4 v = m_value.evaluate(t);

    return glm::mat2( v[0], v[1], v[2], v[3] );
}

} // model
} // bv