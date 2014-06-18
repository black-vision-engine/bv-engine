#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

namespace bv { namespace model
{

class RendererContext;

class IPixelShaderChannel : public IShaderChannel
{
public:

    virtual RendererContextConstPtr     GetRendererContext  () const = 0;

    virtual                             ~IPixelShaderChannel() {};

};

DEFINE_PTR_TYPE(IPixelShaderChannel)
DEFINE_CONST_PTR_TYPE(IPixelShaderChannel)

} // model
} // bv
