#pragma once

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"

namespace bv { namespace model {

class TransformChannel;
class VertexAttributesChannel;
class IGeometryShaderChannel;
class ParamTransform;
class ConnectedComponent;

class ChannelsFactory
{
public:

    static VertexAttributesChannel *            CreateVertexAttributesChannel       ( ConnectedComponent * connComp );
    static TransformChannel *                   CreateTransformChannel              ( const ParamTransform& transformation );
    static IGeometryShaderChannel *             CreateGeometryShaderExtrude         ( float scale );

};

} // model
} // bv