#pragma once

#include "IChannel.h"

#include "Engine\Types\Enums.h"

#include <vector>

namespace bv { namespace model
{

class IPlugin;
class IConnectedComponent;
class IGeometryChannelDescriptor;

class IGeometryChannel : public IChannel
{
public:

    virtual const IGeometryChannelDescriptor *      GetDescriptor       ()                              const = 0;
    virtual PrimitiveType                           GetPrimitiveType    ()                              const = 0;

    virtual int                                     GetNumPrimitives    ( IConnectedComponent * cc )    const = 0;
    virtual std::vector< IConnectedComponent* >     GetComponents       ()                              const = 0;

    virtual void                                    AddConnectedComponent( IConnectedComponent * cc )   = 0;

    virtual bool                                    IsTimeInvariant     ()                              const = 0;

    //FIXME: implement via plugin when its interface is known
    //virtual bool                                  CanBeConnectedTo    ( IPlugin * plugin )            const = 0;
    virtual bool                                    CanBeConnectedTo    ( IGeometryChannel * channel )  const = 0;

    virtual                                         ~IGeometryChannel   (){};
};

} // model
} // bv