#pragma once

#include <string>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IAttributeChannelDescriptor;

class IAttributeChannel : public IChannel
{
public:

    virtual const IAttributeChannelDescriptor *     GetDescriptor   ()  const = 0;

    virtual std::string                             GetName         ()  const = 0;
    
    virtual unsigned int                            GetNumEntries   ()  const = 0;
    
    virtual const char *                            GetData         ()  const = 0;

};

} // model
} // bv
