#include "GeometryChannelUtils.h"

#include <cassert>
#include <cstdarg>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"

namespace bv { namespace model {

// *********************************
// FIXME: make sure that GeometryChannelDescriptor's copy constructor works as expceted
GeometryChannelDescriptor   DescriptorFromConnectedComponent( const ConnectedComponent * cc )
{
    GeometryChannelDescriptor desc;

    for( auto compDesc : cc->GetVertexAttributeChannels() )
    {
        desc.AddVertexAttrChannelDesc( static_cast< const model::VertexAttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
    }

    return desc;
}

// *********************************
// FIXME: basic tests that topology, descriptors and gemoetry fit together
void           ChannelFromConnectedComponents  ( GeometryChannel * channel, const std::vector< ConnectedComponent * > & connectedComponents )
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
void        ChannelFromConnectedComponents  ( GeometryChannel * channel, unsigned int numComponents, ... )
{
    va_list args;
    va_start( args, numComponents );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i < numComponents; ++i )
    {
        ConnectedComponent * cc = va_arg( args, ConnectedComponent* );

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
