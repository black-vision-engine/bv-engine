#include "GeometryPluginRing.h"

#include "System/Print.h"

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
GeometryRingPlugin::GeometryRingPlugin                    ()
    : BasePlugin( nullptr )
{
}

// *************************************
//
GeometryRingPlugin::~GeometryRingPlugin   ()
{
}

// *************************************
//
void                GeometryRingPlugin::Update              ( float t )
{
    BasePlugin::Update( t );
}

// *************************************
//
void                GeometryRingPlugin::Print               ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv
