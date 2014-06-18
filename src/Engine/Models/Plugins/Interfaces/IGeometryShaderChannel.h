#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"


namespace bv { namespace model
{

class IGeometryShaderChannel : public IShaderChannel
{
public:

    virtual         ~IGeometryShaderChannel(){};

};

DEFINE_PTR_TYPE(IGeometryShaderChannel)
DEFINE_CONST_PTR_TYPE(IGeometryShaderChannel)

} // model
} // bv
