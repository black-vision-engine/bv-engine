#pragma once

#include "IChannel.h"
#include <vector>

namespace bv { namespace model
{

class Transform;

class ITransformChannel : public IChannel
{
public:
    virtual void                                    Update( float t )               = 0;

    virtual const std::vector< Transform* >&         GetTransformChannels() const   = 0;


    virtual         ~ITransformChannel(){};
};

} // model
} // bv