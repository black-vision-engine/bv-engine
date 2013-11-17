#include "GeometryPluginRing.h"

#include "System/Print.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RingComponent.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Parameter.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string GeometryRingPluginPD::pluginName( "GeometryRingPlugin" );


// *************************************
//
GeometryRingPluginPD::GeometryRingPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryRingPlugin::GeometryRingPlugin                    ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum )
    : BasePlugin( nullptr )
{
    // Set Geometry Channel
    model::RingComponent * ring = model::RingComponent::Create( startAngle, endAngle, innerRadius, outerRadius, segmentsNum );

    m_geomChannel               = model::PluginsFactory::CreateGeometryChannel( ring );
}

// *************************************
//
GeometryRingPlugin::~GeometryRingPlugin   ()
{
}

// *************************************
//
const IGeometryChannel *    GeometryRingPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *************************************
//
void                GeometryRingPlugin::Update              ( float t )
{
    m_geomChannel->Update( t );
}

// *************************************
//
void                GeometryRingPlugin::Print               ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv
