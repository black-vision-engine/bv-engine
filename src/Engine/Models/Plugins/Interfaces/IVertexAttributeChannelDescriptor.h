#pragma once

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IVertexAttributeChannelDescriptor
{
public:

    virtual AttributeType       GetType         ()  const = 0;
    virtual AttributeSemantic   GetSemantic     ()  const = 0;
    virtual unsigned int        GetEntrySize    ()  const = 0;

    virtual ChannelRole         GetChannelRole  ()  const = 0;

    virtual ~IVertexAttributeChannelDescriptor  () {}

};

} //model
} //bv
