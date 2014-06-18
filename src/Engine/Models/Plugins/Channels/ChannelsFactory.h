#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Transform/TransformChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"

namespace bv { namespace model {

class TransformChannel;
class VertexAttributesChannel;
class ParamTransformVec;

class ChannelsFactory
{
public:

    static VertexAttributesChannelPtr           CreateVertexAttributesChannel       ( ConnectedComponentPtr connComp, bool isTimeInvariant );
    static TransformChannelPtr                  CreateTransformChannel              ( const ParamTransformVec & transformVec );
    static IGeometryShaderChannelPtr            CreateGeometryShaderExtrude         ( float scale, ITimeEvaluatorPtr timeEvaluator );

};

} // model
} // bv