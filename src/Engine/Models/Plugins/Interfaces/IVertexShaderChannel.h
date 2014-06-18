#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"


namespace bv { namespace model
{

class IVertexShaderChannel : public IShaderChannel
{
public:

    virtual         ~IVertexShaderChannel(){};

};

DEFINE_PTR_TYPE(IVertexShaderChannel)
DEFINE_CONST_PTR_TYPE(IVertexShaderChannel)

} // model
} // bv
