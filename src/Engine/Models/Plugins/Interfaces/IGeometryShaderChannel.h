#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"


namespace bv { namespace model
{

class IGeometryShaderChannel : public IShaderChannel
{
public:

    virtual         ~IGeometryShaderChannel(){};
};

} // model
} // bv