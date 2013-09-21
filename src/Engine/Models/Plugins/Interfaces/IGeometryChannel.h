#pragma once

#include "IChannel.h"

#include "Engine\Types\Enums.h"

#include <vector>

namespace bv { namespace model
{

class IConnectedComponent;

class IGeometryChannel : public IChannel
{
public:
    virtual const std::vector< AttrType >&          GetType             () const = 0;
    virtual const std::vector< AttrSemantic >&      GetSemantic         () const = 0;
    virtual PrimitiveType                           GetPrimitiveType    () const = 0;

    virtual int                                     GetNumPrimitives    () const = 0;
    virtual std::vector< IConnectedComponent* >     GetComponents       () const = 0;

    virtual bool                                    IsTimeInvariant     () const = 0;

    virtual                                         ~IGeometryChannel   (){};
};

} // model
} // bv