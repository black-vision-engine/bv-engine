#include "GeometryPluginRect.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Parameter.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/PluginsFactory.h"


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
    model::RectComponent*   rect    = model::RectComponent::Create( w.evaluate( 0.f ), h.evaluate( 0.f ) );
    m_geomChannel                   = PluginsFactory::CreateGeometryChannel( rect );
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
