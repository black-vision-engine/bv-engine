#pragma once

#include "Engine/Models/Plugins/Interfaces/IChannel.h"

#include <vector>


namespace bv { namespace model
{

class Transform;

class ITransformChannel : public IChannel
{
public:

    virtual const std::vector< Transform * > &      GetTransformChannels()  const   = 0;

    virtual         ~ITransformChannel(){};

};

} // model
} // bv
