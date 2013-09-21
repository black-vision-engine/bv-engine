#pragma once

#include "IChannel.h"

namespace bv { namespace model
{

class IConnectedComponent;

class IGeometryChannel : public IChannel
{
public:
    virtual void    AddConnectedComponent( IConnectedComponent* cc ) = 0;

    virtual         ~IGeometryChannel();
};

} // model
} // bv