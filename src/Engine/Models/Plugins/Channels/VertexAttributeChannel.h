#pragma once

#include "Engine/Models/Plugins/Interfaces/IVertexAttributeChannel.h"
#include "Engine\Types\Enums.h"

namespace bv { namespace model
{

class VertexAttributeChannel : public IVertexAttributeChannel
{
public:
    virtual AttrType                GetType()           const;
    virtual AttrSemantic            GetSemantic()       const;

    virtual int                     GetNumEntries()     const;
    virtual const char*             GetData()           const;
};

} // model
} // bv