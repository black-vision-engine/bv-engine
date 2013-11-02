#pragma once

#include "IShaderChannel.h"

//FIXME: add renderer state to this channel (alpha b;lending, z-sorting, stencil and so on)
namespace bv { namespace model
{

class IPixelShaderChannel : public IShaderChannel
{
public:

    virtual         ~IPixelShaderChannel(){};
};

} // model
} // bv