#include "GeometryPluginRect.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Parameter.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string GeometryRectPluginPD::pluginName( "GeometryRectPlugin" );


// *************************************
//
GeometryRectPluginPD::GeometryRectPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryRectPlugin::GeometryRectPlugin                    ( const FloatInterpolator& w, const FloatInterpolator& h )
    : BasePlugin( nullptr )
    , m_width( w )
    , m_height( h )
{
}

// *************************************
//
GeometryRectPlugin::~GeometryRectPlugin   ()
{
}

// *************************************
//
void                GeometryRectPlugin::Update              ( float t )
{
    BasePlugin::Update( t );
}

// *************************************
//
void                GeometryRectPlugin::Print               ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv
