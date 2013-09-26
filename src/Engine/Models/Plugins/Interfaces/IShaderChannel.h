#pragma once

#include "IChannel.h"

#include <string>

namespace bv { namespace model
{

class IShaderChannel : public IChannel
{
public:
    virtual const std::string&      GetShaderFile()         const = 0;

    virtual                         ~IShaderChannel(){};
};

} // model
} // bv