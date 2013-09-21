#pragma once

#include "IChannel.h"

namespace bv { namespace model
{

class Transform;

class ITransformChannel : public IChannel
{
public:
    virtual         ~ITransformChannel(){};
};

} // model
} // bv