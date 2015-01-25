 #pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IConnectedComponent
{
public:

    virtual std::vector< IAttributeChannelPtr >     GetAttributeChannels    () const = 0;
    virtual unsigned int                            GetNumVertices          () const = 0;

    virtual                                         ~IConnectedComponent    (){};

};

DEFINE_PTR_TYPE(IConnectedComponent)
DEFINE_CONST_PTR_TYPE(IConnectedComponent)

} // model
} // bv
