 #pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IAttributeChannel;

class IConnectedComponent
{
public:

    virtual std::vector< IAttributeChannel* >       GetAttributeChannels    () const = 0;
    virtual unsigned int                            GetNumVertices          () const = 0;

    virtual                                         ~IConnectedComponent    (){};

};

} // model
} // bv
