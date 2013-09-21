#pragma once

#include "IChannel.h"

namespace bv { namespace model
{

class Transform;

class ITransformChannel : public IChannel
{
public:

    virtual void    AddTransform( Transform* transform ) = 0;

    virtual         ~ITransformChannel();
};

} // model
} // bv