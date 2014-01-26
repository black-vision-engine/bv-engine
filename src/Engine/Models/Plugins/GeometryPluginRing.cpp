#include "GeometryPluginRing.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"


namespace bv { namespace model {

// ***************************** PLUGIN ********************************** 

// *************************************
//
GeometryRingPlugin::GeometryRingPlugin                    ( float startAngle, float endAngle, float innerRadius, float outerRadius, int segmentsNum )
    : BasePlugin( nullptr )
{
    // Set Geometry Channel
    model::RingComponent * ring = RingComponent::Create( startAngle, endAngle, innerRadius, outerRadius, segmentsNum );

    m_geomChannel               = ChannelsFactory::CreateVertexAttributesChannel( ring );
}

// *************************************
//
GeometryRingPlugin::~GeometryRingPlugin   ()
{
    delete m_geomChannel;
}

// *************************************
//
const IVertexAttributesChannel *    GeometryRingPlugin::GetVertexAttributesChannel          () const
{
    return m_geomChannel;
}

// *************************************
//
void                GeometryRingPlugin::Update              ( TimeType t )
{
    m_geomChannel->Update( t );
}

// *************************************
//
void                GeometryRingPlugin::Print               ( std::ostream & out, int tabs ) const
{
}


} // model
} // bv
