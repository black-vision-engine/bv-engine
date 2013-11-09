#pragma once

#include "IShaderChannel.h"

//FIXME: add renderer state to this channel (alpha b;lending, z-sorting, stencil and so on)
namespace bv { namespace model
{

class RendererContext;

class IPixelShaderChannel : public IShaderChannel
{
public:

    virtual const RendererContext *     GetRendererContext  () const = 0;

    virtual                             ~IPixelShaderChannel(){};

};

} // model
} // bv