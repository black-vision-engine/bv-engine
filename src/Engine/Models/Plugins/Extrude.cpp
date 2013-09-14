#include "Extrude.h"

#include "Engine/Models/Parameter.h"

namespace bv
{

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string ExtrudeParamDescriptor::pluginName( "Extrude" );

//PLUGIN PARAMETERS
const std::string ExtrudeParamDescriptor::scaleParamName( "scale" );

// ******************************
//
ExtrudeParamDescriptor::ExtrudeParamDescriptor()
    : BaseParametersDescriptor( pluginName )
{
    m_params[ scaleParamName ]  = ParamType::PT_FLOAT;
}


// ***************************** PLUGIN **********************************

// ******************************
//
ExtrudePlugin::ExtrudePlugin( const FloatInterpolator & scale )
{
    //FIXME: pass params in constructor
    m_scaleParam    = new ParamFloat( ParamDesc::scaleParamName, scale );
    m_scaleValue    = new ValueFloat( ParamDesc::scaleParamName );

    PluginParamDesc().ValidateParameters( GetShaderFile() );

    RegisterValue( m_scaleValue );
}

// ******************************
//
ExtrudePlugin::~ExtrudePlugin()
{
    delete m_scaleParam;
    delete m_scaleValue;
}

// ******************************
//
std::string ExtrudePlugin::GetShaderFile() const
{
    return "../dep/media/shaders/extrude.geom";
}

// ******************************
//
void ExtrudePlugin::Update( float t )
{
    m_scaleValue->SetValue( m_scaleParam->Evaluate( t ) );
}

};
