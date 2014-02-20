#include "SolidColorShaderChannel.h"


namespace bv { namespace model
{

const std::string SolidColorShaderChannelPD::solidColor = "color";

// ******************************
//
SolidColorShaderChannel::SolidColorShaderChannel( const std::string & shaderFile, const ParamVec4 & color )
    : DefaultPixelShaderChannel( shaderFile, nullptr )
    , m_solidColor(color)
{
//    m_solidColorVal = ValueVec4Ptr( new model::ValueVec4( ParamDesc::solidColor ) );
//    RegisterValue(m_solidColorVal.get());
}

// ******************************
//
void             SolidColorShaderChannel::Update( TimeType t )
{
    //m_solidColorVal->SetValue( m_solidColor.Evaluate( t ) );
    //ShaderChannel::Update( t );
}

// ******************************
//
SolidColorShaderChannel::~SolidColorShaderChannel()
{
}

} // model
} // bv
