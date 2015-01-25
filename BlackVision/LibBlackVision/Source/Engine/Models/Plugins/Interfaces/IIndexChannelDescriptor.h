#pragma once

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IIndexChannelDescriptor
{
public:

    virtual IndexType       GetType                 ()  const = 0;
    virtual int             GetIndexSize            ()  const = 0;

    virtual                 ~IIndexChannelDescriptor() {}

};

} //model
} //bv
