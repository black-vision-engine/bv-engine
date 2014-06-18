#include "VertexAttributesChannelUtils.h"

#include <cassert>
#include <cstdarg>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"


namespace bv { namespace model {

// *********************************
// FIXME: make sure that VertexAttributesChannelDescriptor's copy constructor works as expceted
VertexAttributesChannelDescriptor   DescriptorFromConnectedComponent( ConnectedComponentConstPtr cc )
{
    VertexAttributesChannelDescriptor desc;

    for( auto compDesc : cc->GetAttributeChannels() )
    {
        desc.AddAttrChannelDesc( static_cast< const model::AttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    return desc;
}

// *********************************
// FIXME: basic tests that topology, descriptors and gemoetry fit together
void           ChannelFromConnectedComponents  ( VertexAttributesChannelPtr channel, const std::vector< ConnectedComponentPtr > & connectedComponents )
{
    assert( connectedComponents.size() > 0 );

    channel->SetDescriptor( DescriptorFromConnectedComponent( connectedComponents[ 0 ] ) );
    
    for( auto cc : connectedComponents )
    {
        channel->AddConnectedComponent( cc );
    }
}

// *********************************
// FIXME: basic tests that topology, descriptors and gemoetry fit together
void        ChannelFromConnectedComponents  ( VertexAttributesChannelPtr channel, unsigned int numComponents, ... )
{
    va_list args;
    va_start( args, numComponents );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i < numComponents; ++i )
    {
        ConnectedComponentPtr cc = va_arg( args, ConnectedComponentPtr );

        if ( i == 0 )
        {
            channel->SetDescriptor( DescriptorFromConnectedComponent( cc ) );
        }

        channel->AddConnectedComponent( cc );
    }

    va_end( args );
}

}
}
