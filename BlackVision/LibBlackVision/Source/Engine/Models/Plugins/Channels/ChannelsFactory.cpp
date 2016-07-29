#include "stdafx.h"

#include "ChannelsFactory.h"

#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"
#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *******************************
//
VertexAttributesChannelPtr       ChannelsFactory::CreateVertexAttributesChannel               ( ConnectedComponentPtr connComp, bool isTimeInvariant )
{
	VertexAttributesChannelDescriptor desc;

	for( auto compDesc : connComp->GetAttributeChannels() )
	{
		desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
	}

    VertexAttributesChannelPtr ret = std::make_shared< model::VertexAttributesChannel >( PrimitiveType::PT_TRIANGLE_STRIP, desc, false, isTimeInvariant );

	ret->AddConnectedComponent( connComp );

	return ret;
}

// *******************************
//
TransformChannelPtr              ChannelsFactory::CreateTransformChannel              ( const ParamTransform & transform )
{

    { transform; } // FIXME: suppress unused variable
	//SimpleTransformChannel*      trasformChannel  = new SimpleTransformChannel( transformVec );
    SimpleTransformChannelPtr      trasformChannel  = nullptr; //FIXME: NM_CONFORMANCE
    return trasformChannel;
}

// *******************************
//
IGeometryShaderChannelPtr        ChannelsFactory::CreateGeometryShaderExtrude         ( float scale, ITimeEvaluatorPtr timeEvaluator )
{
    auto extrudeScale = ParametersFactory::CreateParameter( "scale", InterpolatorsHelper::CreateConstValue( scale ), timeEvaluator );
    //return std::make_shared< ExtrudeGeometryShaderChannel >("Assets/Shaders/Deprecated/extrude.geom", extrudeScale);
    return nullptr; //FIXME: remove, dead code
}

} // model
} // bv