#pragma once

#include <string>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IAttributeChannelDescriptor;
DEFINE_CONST_PTR_TYPE( IAttributeChannelDescriptor );

class IAttributeChannel : public IChannel
{
public:

    virtual IAttributeChannelDescriptorConstPtr     GetDescriptor   ()  const = 0;

    virtual std::string                             GetName         ()  const = 0;
    
    virtual unsigned int                            GetNumEntries   ()  const = 0;
    
    virtual const char *                            GetData         ()  const = 0;

    virtual ~IAttributeChannel() {}
};

DEFINE_PTR_TYPE(IAttributeChannel)
DEFINE_CONST_PTR_TYPE(IAttributeChannel)

} // model
} // bv
