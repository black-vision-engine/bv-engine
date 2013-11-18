#pragma once

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IAttributeChannelDescriptor
{
public:

    virtual AttributeType       GetType         ()  const = 0;
    virtual AttributeSemantic   GetSemantic     ()  const = 0;
    virtual unsigned int        GetEntrySize    ()  const = 0;

    virtual ChannelRole         GetChannelRole  ()  const = 0;

    virtual ~IAttributeChannelDescriptor  () {}

};

} //model
} //bv
