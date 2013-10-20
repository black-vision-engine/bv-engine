#include "GeometryChannelAnimatedVertices.h"

#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannelUtils.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/AnimatedStripComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"

namespace bv { namespace model {

// ******************************
//
GeometryChannelAnimatedVertices::GeometryChannelAnimatedVertices     ()
    : GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP )
{
}

// ******************************
//
GeometryChannelAnimatedVertices::~GeometryChannelAnimatedVertices    ()
{
}

// ******************************
//
bool    GeometryChannelAnimatedVertices::NeedsPositionsUpdate        ( float t ) const
{
    return true;
}

// ******************************
//
GeometryChannelAnimatedVertices *   GeometryChannelAnimatedVertices::Create  ( float w, float h, float z, unsigned int numSegments )
{
    GeometryChannelAnimatedVertices * channel = new GeometryChannelAnimatedVertices();
    AnimatedStripComponent * cc = AnimatedStripComponent::Create( w, h, numSegments, z );

    ChannelFromConnectedComponents( channel, 1, cc );

    return channel;
}

} //model
} //bv
