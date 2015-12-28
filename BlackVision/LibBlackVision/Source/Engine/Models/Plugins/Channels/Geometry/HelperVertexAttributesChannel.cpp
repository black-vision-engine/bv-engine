#include "HelperVertexAttributesChannel.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Application/ApplicationContext.h"

namespace bv { namespace model
{

// *************************************
//
void		HelperVertexAttributesChannel::SetAttributesUpdate		( VertexAttributesChannelPtr vaChannel )
{
	if( vaChannel )
	{
		vaChannel->SetAttributesUpdateID( ApplicationContext::Instance().GetTimestamp() + 1 );
	}
}

// *************************************
//
bool		HelperVertexAttributesChannel::PropagateAttributesUpdate		( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin )
{
	if( vaChannel )
	{
		if( prevPlugin && prevPlugin->GetVertexAttributesChannel() )
		{
			auto prevUpdateId = prevPlugin->GetVertexAttributesChannel()->GetAttributesUpdateID();
			if( vaChannel->GetAttributesUpdateID() < prevUpdateId )
			{
				
				vaChannel->SetAttributesUpdateID( prevUpdateId );
				return true;
			}
		}
	}
	return false;
}

// *************************************
//
void		HelperVertexAttributesChannel::SetTopologyUpdate		( VertexAttributesChannelPtr vaChannel )
{
	if( vaChannel )
	{
		vaChannel->SetTopologyUpdateID( ApplicationContext::Instance().GetTimestamp() + 1 );
	}
}

// *************************************
//
bool		HelperVertexAttributesChannel::PropagateTopologyUpdate		( VertexAttributesChannelPtr vaChannel, IPluginPtr prevPlugin )
{
	if( vaChannel )
	{
		if( prevPlugin && prevPlugin->GetVertexAttributesChannel() )
		{
			auto prevUpdateId = prevPlugin->GetVertexAttributesChannel()->GetTopologyUpdateID();
			if( vaChannel->GetTopologyUpdateID() < prevUpdateId )
			{
				vaChannel->SetTopologyUpdateID( prevUpdateId );
				return true;
			}
		}
		return false;
	}
	return ( prevPlugin && prevPlugin->GetVertexAttributesChannel() );
}

} // model
} // bv
