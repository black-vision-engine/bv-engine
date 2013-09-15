#include "PluginGeometryUVSingle.h"

namespace bv
{
// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string PluginGeometryUVSinglePD::pluginName( "geometry.uvsingle" );

// *************************************
//
PluginGeometryUVSinglePD::PluginGeometryUVSinglePD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *********************************
//
PluginGeometryUVSingle::PluginGeometryUVSingle  ( IGeometryGenPlugin * pPrev )
    : m_predcessor( pPrev )
{
    SetRole( Role::R_PROCESSOR );
}

// *********************************
//
PluginGeometryUVSingle::~PluginGeometryUVSingle () 
{
}

}
