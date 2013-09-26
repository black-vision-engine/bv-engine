#pragma once

#include "IShaderChannel.h"

namespace bv { namespace model
{

class IPixelShaderChannel : public IShaderChannel
{
public:
    virtual         ~IPixelShaderChannel(){};
};

} // model
} // bv