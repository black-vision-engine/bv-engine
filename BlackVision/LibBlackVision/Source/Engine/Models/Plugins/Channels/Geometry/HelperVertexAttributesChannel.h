#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

namespace bv { namespace model
{

class HelperVertexAttributesChannel
{
public:
	static void									SetAttributesUpdate			( VertexAttributesChannelPtr vaChannel );
	static bool									PropagateAttributesUpdate	( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin );
	static void									SetTopologyUpdate			( VertexAttributesChannelPtr vaChannel );
	static bool									PropagateTopologyUpdate		( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin );

};

} // model
} // bv
