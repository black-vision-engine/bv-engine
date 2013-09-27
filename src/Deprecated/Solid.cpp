#include "Solid.h"
#include "Engine/Models/Plugins/Parameter.h"

#include "System/Print.h"

namespace bv {

//// ***************************** DESCRIPTOR **********************************
////PLUGIN NAME
//const std::string SolidParametersDescriptor::pluginName( "Solid" );
//
////PLUGIN PARAMETERS
//const std::string SolidParametersDescriptor::colorParamName( "solidColod" );
//
//// ******************************
////
//SolidParametersDescriptor::SolidParametersDescriptor()
//    : BaseParametersDescriptor( pluginName )
//{
//    m_params[ colorParamName ] = ParamType::PT_FLOAT4;
//}
//
//
//// ***************************** PLUGIN **********************************
//
//// ******************************
////
//SolidColorPlugin::SolidColorPlugin( const Vec4Interpolator & col )
//{
//    //FIXME: pass params in constructor
//    m_colorParam    = new ParamVec4( ParamDesc::colorParamName, col );
//    m_colorValue    = new ValueVec4( ParamDesc::colorParamName );
//
//    //FIXME: GetShaderResource should be used instead
//    PluginParamDesc().ValidateParameters( GetShaderFile() );
//
//    RegisterValue( m_colorValue );
//}
//
//// ******************************
////
//SolidColorPlugin::~SolidColorPlugin   ()
//{
//    delete m_colorParam;
//    delete m_colorValue;
//}
//
////GetModelParamList() //TODO
//
//// ******************************
////
//std::string SolidColorPlugin::GetShaderFile () const
//{
//    // TODO; do not bind Solid with exact shader - at some point it will be provided by GLSL shader service or whatever else resource manager
//    return "../dep/media/shaders/solid.frag";
//}
//
//// ******************************
////
//void SolidColorPlugin::Update(float t)
//{
//    m_colorValue->SetValue( m_colorParam->Evaluate( t ) );
//}
//
//// ******************************
////
//void SolidColorPlugin::Print(std::ostream& out, int tabs) const
//{
//    out << GetName() << debug::EndLine(tabs);
//    out << "PARAMS:" << debug::EndLine(tabs);
//    const glm::vec4& val = m_colorValue->GetValue();
//    out << m_colorParam->getName() << "  " << val.a<< " " << val.r<< " " << val.g<< " " << val.b << debug::EndLine(tabs);
//}

} // bv
