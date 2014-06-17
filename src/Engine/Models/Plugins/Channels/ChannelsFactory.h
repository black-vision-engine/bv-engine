#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"

namespace bv { namespace model {

class TransformChannel;
class VertexAttributesChannel;
class IGeometryShaderChannel;
class ParamTransformVec;
class ConnectedComponent;

class ChannelsFactory
{
public:

    static VertexAttributesChannelPtr           CreateVertexAttributesChannel       ( ConnectedComponent * connComp, bool isTimeInvariant );
    static TransformChannel *                   CreateTransformChannel              ( const ParamTransformVec & transformVec );
    static IGeometryShaderChannel *             CreateGeometryShaderExtrude         ( float scale, ITimeEvaluatorPtr timeEvaluator );

};

} // model
} // bv