#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

class DefaultVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
public:

    DefaultVertexShaderChannel();

};

typedef std::shared_ptr< DefaultVertexShaderChannel >    DefaultVertexShaderChannelPtr;

} // model
} // bv
