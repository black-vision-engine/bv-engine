#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryShaderChannel.h"

namespace bv { namespace model {

class DefaultFinalizeGeometryShaderChannel : DefaultFinalizeShaderChannel< IGeometryShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IGeometryShaderChannel > Parent;

private:

    std::string m_shaderSource;

public:


        DefaultFinalizeGeometryShaderChannel      ( IGeometryShaderChannelPtr channel );
        ~DefaultFinalizeGeometryShaderChannel     ();

};

} //model
} //bv
