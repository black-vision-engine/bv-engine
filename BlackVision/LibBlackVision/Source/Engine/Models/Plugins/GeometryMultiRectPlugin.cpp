#include "stdafx.h"

#include "GeometryMultiRectPlugin.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"


namespace bv { namespace model {

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryMultiRectPlugin::GeometryMultiRectPlugin                    ()
    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
    , m_vaChannel( nullptr )
{
}

// *************************************
//
GeometryMultiRectPlugin::~GeometryMultiRectPlugin   ()
{
}

// *************************************
//
IVertexAttributesChannelConstPtr    GeometryMultiRectPlugin::GetVertexAttributesChannel          () const
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
    { tabs; out; } // FIXME: suppress unused variable
}

// *************************************
//
void                        GeometryMultiRectPlugin::AddRectConnectedComponnent  ( const ParamFloat & w, const ParamFloat & h, const ParamFloat & tx, const ParamFloat & ty, const ParamFloat & tz )
{
    AddRectConnectedComponnent( w.Evaluate(), h.Evaluate(), tx.Evaluate(), ty.Evaluate(), tz.Evaluate() );
}

void                        GeometryMultiRectPlugin::AddRectConnectedComponnent  ( float w, float h, float tx, float ty, float tz )
{
    auto  rect      = model::RectComponent::Create( w, h, tx, ty, tz );
    if( !m_vaChannel )
    {
        m_vaChannel = ChannelsFactory::CreateVertexAttributesChannel( rect, true );
    }
    else
    {
        m_vaChannel->AddConnectedComponent( rect );
    }
}

} // model
} // bv
