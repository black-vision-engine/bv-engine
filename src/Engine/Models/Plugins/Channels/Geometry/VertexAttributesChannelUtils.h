#pragma once

#include <vector>

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

namespace bv { namespace model {

class ConnectedComponent;
class VertexAttributesChannel;

VertexAttributesChannelDescriptor   DescriptorFromConnectedComponent( ConnectedComponentConstPtr cc );

void                        ChannelFromConnectedComponents  ( VertexAttributesChannel * channel, const std::vector< ConnectedComponent * > & connectedComponents );
void                        ChannelFromConnectedComponents  ( VertexAttributesChannel * channel, unsigned int numComponents, ... );


} //model
} //bv
