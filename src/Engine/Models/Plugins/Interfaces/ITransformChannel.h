#pragma once

#include "Engine/Models/Plugins/Interfaces/IChannel.h"

#include <vector>


namespace bv { namespace model
{

class Transform;

typedef std::shared_ptr< Transform >    TransformPtr;

class ITransformChannel : public IChannel
{
public:

    virtual const std::vector< TransformPtr > &      GetTransformChannels()  const   = 0;

    virtual         ~ITransformChannel(){};

};

} // model
} // bv
