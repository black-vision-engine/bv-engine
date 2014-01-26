#include "GeometryMultiRectPlugin.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"


namespace bv { namespace model {

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryMultiRectPlugin::GeometryMultiRectPlugin                    ()
    : BasePlugin( nullptr )
    , m_vaChannel( nullptr )
{
}

// *************************************
//
GeometryMultiRectPlugin::~GeometryMultiRectPlugin   ()
{
    delete m_vaChannel;
}

// *************************************
//
const IVertexAttributesChannel *    GeometryMultiRectPlugin::GetVertexAttributesChannel          () const
{
    return m_vaChannel;
}

// *************************************
//
void                        GeometryMultiRectPlugin::Update              ( TimeType t )
{
    m_vaChannel->Update( t );
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
    if( !m_vaChannel )
    {
        m_vaChannel                   = ChannelsFactory::CreateVertexAttributesChannel( rect );
    }
    else
    {
        m_vaChannel->AddConnectedComponent( rect );
    }
}

} // model
} // bv
