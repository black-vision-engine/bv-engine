#include "SolidColorShaderChannel.h"

namespace bv { namespace model
{

const std::string SolidColorShaderChannelPD::pluginName = "Solid";
const std::string SolidColorShaderChannelPD::solidColor = "color";

// ******************************
//
SolidColorShaderChannel::SolidColorShaderChannel(const std::string& shaderFile, const Vec4Interpolator& color)
    : PixelShaderChannelBase( shaderFile )
    , m_solidColor(color)
{
    m_solidColorVal = new model::ValueVec4( ParamDesc::solidColor );
    RegisterValue(m_solidColorVal);
}

// ******************************
//
void             SolidColorShaderChannel::Update( TimeType t )
{
    m_solidColorVal->SetValue( m_solidColor.evaluate( t ) );
    ShaderChannel::Update( t );
}

// ******************************
//
SolidColorShaderChannel::~SolidColorShaderChannel()
{
}

} // model
} // bv