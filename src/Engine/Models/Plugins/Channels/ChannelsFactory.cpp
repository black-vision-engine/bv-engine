#include "ChannelsFactory.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/GeometryShader/ExtrudeGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

namespace bv { namespace model {

// *******************************
//
VertexAttributesChannel *       ChannelsFactory::CreateVertexAttributesChannel               ( IConnectedComponent * connComp )
{
	VertexAttributesChannelDescriptor desc;

	for( auto compDesc : connComp->GetAttributeChannels() )
	{
		desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
	}

	VertexAttributesChannel * ret = new model::VertexAttributesChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc );

	ret->AddConnectedComponent(connComp);

	return ret;
}

// *******************************
//
TransformChannel *              ChannelsFactory::CreateTransformChannel              ( const ParamTransform& transformation )
{
	SimpleTransformChannel*      trasformChannel  = new SimpleTransformChannel();
	trasformChannel->AddTransform( transformation );

	return trasformChannel;
}

// *******************************
//
IGeometryShaderChannel *        ChannelsFactory::CreateGeometryShaderExtrude         ( float scale )
{
    auto extrudeScale = ParametersFactory::CreateParameter( "scale", InterpolatorsHelper::CreateConstValue( scale ) );
	return new ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
}

} // model
} // bv