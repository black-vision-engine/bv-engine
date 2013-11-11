#include "GeometryPlugin.h"


namespace bv { namespace model { 

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string GeometryPluginPD::pluginName( "GeometryPlugin" );

// *************************************
//
GeometryPluginPD::GeometryPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}


// ***************************** PLUGIN ********************************** 
// *******************************
//
GeometryPlugin::GeometryPlugin  ()
{
}

// *******************************
//
GeometryPlugin::~GeometryPlugin ()
{
}

// *******************************
//
void  GeometryPlugin::Update          ( float t )
{
    BasePlugin::Update( t );
}

// *******************************
//
void  GeometryPlugin::Print           ( std::ostream & out, int tabs ) const
{
}

}
}
