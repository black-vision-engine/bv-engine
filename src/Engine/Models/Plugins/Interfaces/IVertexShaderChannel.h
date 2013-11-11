#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"


namespace bv { namespace model
{

class IVertexShaderChannel : public IShaderChannel
{
public:
    virtual         ~IVertexShaderChannel(){};
};


} // model
} // bv