#pragma once

#include "IChannel.h"
#include "Engine\Types\Enums.h"

namespace bv { namespace model
{

class IVertexAttributeChannel : public IChannel
{
public:
    virtual AttrType                GetType()           const = 0;
    virtual AttrSemantic            GetSemantic()       const = 0;

    virtual int                     GetNumEntries()     const = 0;
    virtual const char*             GetData()           const = 0;
};

} // model
} // bv