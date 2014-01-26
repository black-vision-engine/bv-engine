#include "GeometryMultiRectPlugin.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"


namespace bv { namespace model {

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
    delete m_geomChannel;
}

// *************************************
//
const IVertexAttributesChannel *    GeometryMultiRectPlugin::GetVertexAttributesChannel          () const
{
    return m_geomChannel;
}

// *************************************
//
void                        GeometryMultiRectPlugin::Update              ( TimeType t )
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
void                        GeometryMultiRectPlugin::AddRectConnectedComponnent  ( const ParamFloat& w, const ParamFloat& h, const ParamFloat& tx, const ParamFloat& ty, const ParamFloat& tz )
{
    AddRectConnectedComponnent( w.Evaluate( 0.f ), h.Evaluate( 0.f ), tx.Evaluate( 0.f ), ty.Evaluate( 0.f ), tz.Evaluate( 0.f ) );
}

void                        GeometryMultiRectPlugin::AddRectConnectedComponnent  ( float w, float h, float tx, float ty, float tz )
{
    model::RectComponent*   rect    = model::RectComponent::Create( w, h, tx, ty, tz );
    if( !m_geomChannel )
    {
        m_geomChannel                   = ChannelsFactory::CreateVertexAttributesChannel( rect );
    }
    else
    {
        m_geomChannel->AddConnectedComponent( rect );
    }
}

} // model
} // bv
