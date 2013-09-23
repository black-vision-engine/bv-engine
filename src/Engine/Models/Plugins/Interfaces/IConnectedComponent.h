 #pragma once

#include "IChannel.h"
#include "Engine\Types\Enums.h"

#include <vector>

namespace bv { namespace model
{

class IVertexAttributeChannel;

class IConnectedComponent
{
public:

    virtual std::vector< IVertexAttributeChannel* >     GetVertexAttributeChannels  () const = 0;

    virtual int                                         GetNumVertices              () const = 0;

    virtual                                             ~IConnectedComponent        (){};
};

} // model
} // bv