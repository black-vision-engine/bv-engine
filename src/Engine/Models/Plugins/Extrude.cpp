#include "Extrude.h"

#include "Engine/Models/Parameter.h"

namespace bv
{

ExtrudeParamDescriptor::ExtrudeParamDescriptor()
{
    m_scaleParamName = "scale";
    m_params[m_scaleParamName]  = ParamType::PT_FLOAT;
}

ExtrudePlugin::ExtrudePlugin(const FloatInterpolator& scale)
    : BasePlugin("Extrude")
{
    m_paramDesc = new ExtrudeParamDescriptor();
    //FIXME: pass params in constructor
    m_scale         = new ParamFloat(m_paramDesc->m_scaleParamName, scale);
    m_scaleValue    = new ValueFloat(m_paramDesc->m_scaleParamName);

    m_paramDesc->ValidateParameters(GetShaderFile());

    RegisterValue(m_scaleValue);
}

std::string ExtrudePlugin::GetShaderFile() const
{
    return "../dep/media/shaders/extrude.geom";
}

void ExtrudePlugin::Update(float t)
{
    m_scaleValue->SetValue( m_scale->Evaluate( t ) );
}

};