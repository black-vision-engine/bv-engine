#include "HelperVertexAttributesChannel.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"


namespace bv { namespace model
{

// *************************************
//
void		HelperVertexAttributesChannel::SetAttributesUpdate		( VertexAttributesChannelPtr vaChannel )
{
	if( vaChannel )
	{
		vaChannel->SetLastAttributeUpdateID( vaChannel->GetLastAttributeUpdateID() + 1 );
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
			auto prevUpdateId = prevPlugin->GetVertexAttributesChannel()->GetLastAttributeUpdateID();
			if( vaChannel->GetLastAttributeUpdateID() < prevUpdateId )
			{
				vaChannel->SetLastAttributeUpdateID( prevUpdateId );
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
		vaChannel->SetLastTopologyUpdateID( vaChannel->GetLastTopologyUpdateID() + 1 );
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
			auto prevUpdateId = prevPlugin->GetVertexAttributesChannel()->GetLastTopologyUpdateID();
			if( vaChannel->GetLastTopologyUpdateID() < prevUpdateId )
			{
				vaChannel->SetLastTopologyUpdateID( prevUpdateId );
				return true;
			}
		}
		return false;
	}
	return ( prevPlugin && prevPlugin->GetVertexAttributesChannel() );
}

// *************************************
//
VertexAttributesChannelDescriptor		HelperVertexAttributesChannel::CreateVertexAttributesChannelDescriptor		( const std::vector< IAttributeChannelPtr > & attrChannels )
{
	VertexAttributesChannelDescriptor desc;
    for( auto attrChannel : attrChannels )
    {
        auto prevCompChDesc = attrChannel->GetDescriptor();
        desc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
    }
	return desc;
}

} // model
} // bv
