#include "SolidColorPlugin.h"
#include "System/Print.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

#include "Engine/Models/Plugins/Parameter.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SolidColorPluginPD::pluginName( "SolidColorPlugin" );

//PLUGIN PARAMETERS
const std::string SolidColorPluginPD::solidColor( "solidColor" );


// *************************************
//
SolidColorPluginPD::SolidColorPluginPD()
    : BaseParametersDescriptor( pluginName )
{
    m_params[ solidColor ] = ParamType::PT_FLOAT4;
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SolidColorPlugin::SolidColorPlugin                    ( const IPlugin* prev )
{
    m_geomChannel       = nullptr;
    m_transformChannel  = nullptr;

    m_geomChannel       = const_cast< IGeometryChannel* >( prev->GetGeometryChannel() ); // FIXME: remove const_cast
    m_transformChannel  = const_cast< ITransformChannel* >( prev->GetTransformChannel() );
}

// *************************************
//
SolidColorPlugin::~SolidColorPlugin   ()
{

}

// *************************************
//
void                SolidColorPlugin::Update              ( float t )
{
    BasePlugin::Update( t );
}

// *************************************
//
void                SolidColorPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

} // model
} // bv
