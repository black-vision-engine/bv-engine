#include "GeometryPluginRect.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"


namespace bv { namespace model {

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryRectPlugin::GeometryRectPlugin                    ( const ParamFloat& w, const ParamFloat& h )
    : BasePlugin( nullptr )
    , m_width( w )
    , m_height( h )
{
    RectComponent*   rect           = RectComponent::Create( w.Evaluate( 0.f ), h.Evaluate( 0.f ) );
    m_vaChannel                   = ChannelsFactory::CreateVertexAttributesChannel( rect );
}

// *************************************
//
GeometryRectPlugin::~GeometryRectPlugin   ()
{
    delete m_vaChannel;
}

// *************************************
//
const IVertexAttributesChannel *    GeometryRectPlugin::GetVertexAttributesChannel          () const
{
    return m_vaChannel;
}

// *************************************
//
void                        GeometryRectPlugin::Update              ( TimeType t )
{
    m_vaChannel->Update( t );
}

// *************************************
//
void                        GeometryRectPlugin::Print               ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv
