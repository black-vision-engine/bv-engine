#include "GeometryPluginRect.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"

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
GeometryRectPlugin::GeometryRectPlugin                    ( const ParamFloat& w, const ParamFloat& h )
    : BasePlugin( nullptr )
    , m_width( w )
    , m_height( h )
{
    RectComponent*   rect           = RectComponent::Create( w.Evaluate( 0.f ), h.Evaluate( 0.f ) );
    m_geomChannel                   = ChannelsFactory::CreateGeometryChannel( rect );
}

// *************************************
//
GeometryRectPlugin::~GeometryRectPlugin   ()
{
}

// *************************************
//
const IVertexAttributesChannel *    GeometryRectPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *************************************
//
void                        GeometryRectPlugin::Update              ( TimeType t )
{
    m_geomChannel->Update( t );
}

// *************************************
//
void                        GeometryRectPlugin::Print               ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv
