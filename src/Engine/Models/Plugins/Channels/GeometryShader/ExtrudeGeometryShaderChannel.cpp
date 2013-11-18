#include "ExtrudeGeometryShaderChannel.h"


namespace bv{ namespace model {

const std::string ExtrudeGeometryShaderChannelPD::pluginName = "PluginName";
const std::string ExtrudeGeometryShaderChannelPD::scaleParamName = "scale";

// ******************************
//
ExtrudeGeometryShaderChannel::ExtrudeGeometryShaderChannel(const std::string& shaderFile, const ParamFloat& scale)
    : ShaderChannel( shaderFile  )
    , m_scale( scale )
{
    m_scaleVal = new model::ValueFloat( ParamDesc::scaleParamName );
    RegisterValue( m_scaleVal );

}

// ******************************
//
ExtrudeGeometryShaderChannel::~ExtrudeGeometryShaderChannel()
{
}

// ******************************
//
void             ExtrudeGeometryShaderChannel::Update( TimeType t )
{
    m_scaleVal->SetValue( m_scale.Evaluate( t ) );
    ShaderChannel::Update( t );
}

} // model
} // bv
