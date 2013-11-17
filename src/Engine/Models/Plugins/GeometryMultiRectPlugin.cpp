#include "GeometryMultiRectPlugin.h"

#include "System/Print.h"

#include "Engine/Models/Resources/TextureLoader.h"
#include "Engine/Models/Plugins/Parameter.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/RectComponent.h"
#include "Engine/Models/Plugins/PluginsFactory.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string GeometryMultiRectPluginPD::pluginName( "GeometryMultiRectPlugin" );


// *************************************
//
GeometryMultiRectPluginPD::GeometryMultiRectPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryMultiRectPlugin::GeometryMultiRectPlugin                    ()
    : BasePlugin( nullptr )
    , m_geomChannel( nullptr )
{
}

// *************************************
//
GeometryMultiRectPlugin::~GeometryMultiRectPlugin   ()
{
}

// *************************************
//
const IGeometryChannel *    GeometryMultiRectPlugin::GetGeometryChannel          () const
{
    return m_geomChannel;
}

// *************************************
//
void                        GeometryMultiRectPlugin::Update              ( float t )
{
    m_geomChannel->Update( t );
}

// *************************************
//
void                        GeometryMultiRectPlugin::Print               ( std::ostream & out, int tabs ) const
{
}

// *************************************
//
void                        GeometryMultiRectPlugin::AddRectConnectedComponnent  ( const FloatInterpolator& w, const FloatInterpolator& h, const FloatInterpolator& tx, const FloatInterpolator& ty, const FloatInterpolator& tz )
{
    AddRectConnectedComponnent( w.evaluate( 0.f ), h.evaluate( 0.f ), tx.evaluate( 0.f ), ty.evaluate( 0.f ), tz.evaluate( 0.f ) );
}

void                        GeometryMultiRectPlugin::AddRectConnectedComponnent  ( float w, float h, float tx, float ty, float tz )
{
    model::RectComponent*   rect    = model::RectComponent::Create( w, h, tx, ty, tz );
    if( !m_geomChannel )
    {
        m_geomChannel                   = PluginsFactory::CreateGeometryChannel( rect );
    }
    else
    {
        m_geomChannel->AddConnectedComponent( rect );
    }
}

} // model
} // bv
