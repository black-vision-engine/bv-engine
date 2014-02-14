#pragma once

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"

namespace bv { namespace model {

class TransformChannel;
class VertexAttributesChannel;
class IGeometryShaderChannel;
class ParamTransformVec;
class ConnectedComponent;

class ChannelsFactory
{
public:

    static VertexAttributesChannel *            CreateVertexAttributesChannel       ( ConnectedComponent * connComp );
    static TransformChannel *                   CreateTransformChannel              ( const ParamTransformVec & transformVec );
    static IGeometryShaderChannel *             CreateGeometryShaderExtrude         ( float scale );

};

} // model
} // bv