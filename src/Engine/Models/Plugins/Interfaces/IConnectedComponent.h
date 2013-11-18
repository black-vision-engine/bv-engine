 #pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IVertexAttributeChannel;

class IConnectedComponent
{
public:

    virtual std::vector< IVertexAttributeChannel* >     GetVertexAttributeChannels  () const = 0;
    virtual unsigned int                                GetNumVertices              () const = 0;

    virtual                                             ~IConnectedComponent        (){};
};

} // model
} // bv
