#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/Simple/GeometryGenerator.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"


namespace bv { namespace model {


// *********************************
//
class DefaultGeometryVertexAttributeChannel : public VertexAttributesChannel
{
private:

    AttributeChannelDescriptorPtr m_compVertDesc;
    AttributeChannelDescriptorPtr m_compNormDesc;
    AttributeChannelDescriptorPtr m_compUVDesc;

public:

            DefaultGeometryVertexAttributeChannel   ( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false );

    void    AddAttributeChannelDescriptors          ( IGeometryGenerator & generator );
    void    GenerateAndAddConnectedComponent        ( IGeometryGenerator & generator );

};

} //model
} //bv