#pragma once

#include "Engine/Models/Plugins/Channels/DefaultFinalizeShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"


namespace bv { namespace model {

class DefaultFinalizePixelShaderChannel : DefaultFinalizeShaderChannel< IPixelShaderChannel >
{
private:

    typedef  DefaultFinalizeShaderChannel< IPixelShaderChannel > Parent;

private:

    std::string m_shaderSource;

public:


        DefaultFinalizePixelShaderChannel       ( IPixelShaderChannelPtr channel );
        ~DefaultFinalizePixelShaderChannel      ();

};

} //model
} //bv
