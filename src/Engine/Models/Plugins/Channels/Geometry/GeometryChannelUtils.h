#pragma once

#include <vector>
#include "Engine\Models\Plugins\Channels\Geometry\GeometryChannelDescriptor.h"


namespace bv { namespace model {

class ConnectedComponent;
class GeometryChannel;

GeometryChannelDescriptor   DescriptorFromConnectedComponent( const ConnectedComponent * cc );

void                        ChannelFromConnectedComponents  ( GeometryChannel * channel, const std::vector< ConnectedComponent * > & connectedComponents );
void                        ChannelFromConnectedComponents  ( GeometryChannel * channel, unsigned int numComponents, ... );


} //model
} //bv
