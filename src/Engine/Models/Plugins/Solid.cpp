#include "Solid.h"
#include "Engine/Models/Parameter.h"

#include "System/Print.h"

namespace bv {

using namespace debug;

SolidParametersDescriptor::SolidParametersDescriptor()
{
    m_colorParamName            = "solidColor";
    m_params[m_colorParamName]  = ParamType::PT_FLOAT4;
}

SolidColorPlugin::SolidColorPlugin(const Vec4Interpolator& col)
    : BasePlugin("Solid")
{
    m_paramDesc = new SolidParametersDescriptor();
    //FIXME: pass params in constructor
    m_color         = new ParamVec4(m_paramDesc->m_colorParamName, col);
    m_colorValue    = new ValueVec4(m_paramDesc->m_colorParamName);

    m_paramDesc->ValidateParameters(GetShaderFile());

    RegisterValue(m_colorValue);
}

//GetModelParamList() //TODO

std::string SolidColorPlugin::GetShaderFile() const
{
    // TODO; do not bind Solid with exact shader
    return "../dep/media/shaders/solid.frag";
}

void SolidColorPlugin::Update(float t)
{
    m_colorValue->SetValue( m_color->Evaluate( t ) );
}

void SolidColorPlugin::Print(std::ostream& out, int tabs) const
{
    out << m_pluginName << EndLine(tabs);
    out << "PARAMS:" << EndLine(tabs);
    const glm::vec4& val = m_colorValue->GetValue();
    out << m_color->getName() << "  " << val.a<< " " << val.r<< " " << val.g<< " " << val.b << EndLine(tabs);
}

} // bv