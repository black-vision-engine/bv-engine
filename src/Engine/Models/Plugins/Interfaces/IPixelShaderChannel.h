#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"


namespace bv { namespace model
{

class RendererContext;

class IPixelShaderChannel : public IShaderChannel
{
public:

    virtual const RendererContext *     GetRendererContext  () const = 0;

    virtual                             ~IPixelShaderChannel() {};

};

} // model
} // bv
