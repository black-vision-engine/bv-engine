#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizeVertexShaderChannel : DefaultFinalizeShaderChannel< IVertexShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IVertexShaderChannel > Parent;

private:

    std::string m_shaderSource;

public:


        DefaultFinalizeVertexShaderChannel      ( IVertexShaderChannelPtr channel );
        ~DefaultFinalizeVertexShaderChannel     ();

};

} //model
} //bv
