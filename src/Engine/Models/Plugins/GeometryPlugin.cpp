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
GeometryPlugin::GeometryPlugin  ( model::VertexAttributesChannel * geomChannel )
    : BasePlugin( nullptr )
    , m_geomChannel( geomChannel )
{
}

// *******************************
//
GeometryPlugin::~GeometryPlugin ()
{
}

// *******************************
//
const IVertexAttributesChannel *   GeometryPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *******************************
//
void  GeometryPlugin::Update          ( TimeType t )
{
    m_geomChannel->Update( t );
}

// *******************************
//
void  GeometryPlugin::Print           ( std::ostream & out, int tabs ) const
{
}

}
}
