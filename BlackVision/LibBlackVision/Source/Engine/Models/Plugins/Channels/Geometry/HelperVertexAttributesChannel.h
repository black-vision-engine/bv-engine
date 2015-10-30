#pragma once

#include <functional>

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

namespace bv { namespace model
{

class HelperVertexAttributesChannel
{
public:
	static bool									SetAttributesUpdate			( VertexAttributesChannelPtr vaChannel, bool stateChanged );
	static bool									PropagateAttributesUpdate	( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin );
	static bool									SetTopologyUpdate			( VertexAttributesChannelPtr vaChannel, bool stateChanged );
	static bool									PropagateTopologyUpdate		( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin );

	static VertexAttributesChannelDescriptor	CreateVertexAttributesChannelDescriptor	( const std::vector< IAttributeChannelPtr > & components );
};

} // model
} // bv
