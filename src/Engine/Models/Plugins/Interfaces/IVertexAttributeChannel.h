#pragma once

#include <string>

#include "IChannel.h"
#include "Engine\Types\Enums.h"

namespace bv { namespace model
{

class IVertexAttributeChannelDescriptor;

class IVertexAttributeChannel : public IChannel
{
public:

    virtual const IVertexAttributeChannelDescriptor *   GetDescriptor   ()  const = 0;

    virtual std::string                                 GetName         ()  const = 0;
    
    virtual unsigned int                                GetNumEntries   ()  const = 0;
    
    virtual const char *                                GetData         ()  const = 0;

};

} // model
} // bv
