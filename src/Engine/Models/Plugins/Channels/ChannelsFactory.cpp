#include "ChannelsFactory.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/GeometryShader/ExtrudeGeometryShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

namespace bv { namespace model {

// *******************************
//
VertexAttributesChannel *       ChannelsFactory::CreateVertexAttributesChannel               ( ConnectedComponent * connComp, bool isTimeInvariant )
{
	VertexAttributesChannelDescriptor desc;

	for( auto compDesc : connComp->GetAttributeChannels() )
	{
		desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
	}

    VertexAttributesChannel * ret = new model::VertexAttributesChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc, false, isTimeInvariant );

	ret->AddConnectedComponent( connComp );

	return ret;
}

// *******************************
//
TransformChannel *              ChannelsFactory::CreateTransformChannel              ( const ParamTransformVec & transformVec )
{

	//SimpleTransformChannel*      trasformChannel  = new SimpleTransformChannel( transformVec );
    SimpleTransformChannel*      trasformChannel  = nullptr; //FIXME: NM_CONFORMANCE
    return trasformChannel;
}

// *******************************
//
IGeometryShaderChannel *        ChannelsFactory::CreateGeometryShaderExtrude         ( float scale, ITimeEvaluatorPtr timeEvaluator )
{
    auto extrudeScale = ParametersFactory::CreateParameter( "scale", InterpolatorsHelper::CreateConstValue( scale ), timeEvaluator );
	return new ExtrudeGeometryShaderChannel("../dep/media/shaders/extrude.geom", extrudeScale);
}

} // model
} // bv