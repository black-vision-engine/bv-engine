#pragma once

#include <functional>

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

namespace bv { namespace model
{

class HelperVertexAttributesChannel
{
public:
	static bool									AttributesUpdate				( VertexAttributesChannelPtr vaChannel, bool stateChanged );
	static bool									FetchAttributesUpdate		( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin );
	static bool									TopologyUpdate					( VertexAttributesChannelPtr vaChannel, bool stateChanged );
	static bool									FetchTopologyUpdate			( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin );

	static VertexAttributesChannelDescriptor	CreateVertexAttributesChannelDescriptor	( const std::vector< IAttributeChannelPtr > & components );
};

} // model
} // bv
