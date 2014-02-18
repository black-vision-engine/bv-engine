#include "ExtrudeGeometryShaderChannel.h"


namespace bv { namespace model {

const std::string ExtrudeGeometryShaderChannelPD::scaleParamName = "scale";

// ******************************
//
ExtrudeGeometryShaderChannel::ExtrudeGeometryShaderChannel(const std::string& shaderFile, const ParamFloat& scale)
    : ShaderChannel( shaderFile, nullptr )
    , m_scale( scale )
{
    //m_scaleVal = ValueFloatPtr( new ValueFloat( ParamDesc::scaleParamName ) );
    //RegisterValue( m_scaleVal.get() );

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
    //m_scaleVal->SetValue( m_scale.Evaluate( t ) );
    //ShaderChannel::Update( t );
}

} // model
} // bv
