#include "SolidColorShaderChannel.h"

namespace bv{ namespace model
{

const std::string SolidColorShaderChannelPD::pluginName = "Solid";
const std::string SolidColorShaderChannelPD::solidColor = "solidColor";

SolidColorShaderChannel::SolidColorShaderChannel(const std::string& shaderFile, const Vec4Interpolator& color)
    : ShaderChannel( shaderFile )
    , m_solidColor(color)
{
    m_solidColorVal = new model::ValueVec4( ParamDesc::solidColor );
    RegisterValue(m_solidColorVal);
}

void             SolidColorShaderChannel::Update( float t )
{
    m_solidColorVal->SetValue( m_solidColor.evaluate( t ) );
    ShaderChannel::Update( t );
}

SolidColorShaderChannel::~SolidColorShaderChannel() {}

} // model
} // bv