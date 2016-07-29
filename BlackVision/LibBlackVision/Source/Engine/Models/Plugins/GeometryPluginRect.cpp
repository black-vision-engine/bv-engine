#include "stdafx.h"

#include "GeometryPluginRect.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryRectPlugin::GeometryRectPlugin                    ( const ParamFloat & w, const ParamFloat & h )
    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
    , m_width( w )
    , m_height( h )
{
    auto              rect  = RectComponent::Create( w.Evaluate(), h.Evaluate() );
    m_vaChannel             = ChannelsFactory::CreateVertexAttributesChannel( rect, true );
}

// *************************************
//
GeometryRectPlugin::~GeometryRectPlugin   ()
{
}

// *************************************
//
IVertexAttributesChannelConstPtr    GeometryRectPlugin::GetVertexAttributesChannel          () const
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
    { tabs; out; } // FIXME: suppress unused variable
}


} // model
} // bv
