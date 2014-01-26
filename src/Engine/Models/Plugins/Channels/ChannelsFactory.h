#pragma once

#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"

namespace bv { namespace model {

class TransformChannel;
class VertexAttributesChannel;
class IGeometryShaderChannel;
class ParamTransform;
class IConnectedComponent;

class ChannelsFactory
{
public:

    static VertexAttributesChannel *            CreateVertexAttributesChannel               ( IConnectedComponent * connComp );
    static TransformChannel *                   CreateTransformChannel              ( const ParamTransform& transformation );
    static IGeometryShaderChannel *             CreateGeometryShaderExtrude         ( float scale );

};

} // model
} // bv