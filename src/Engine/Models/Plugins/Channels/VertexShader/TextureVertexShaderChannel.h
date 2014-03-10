#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

class TextureVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
public:

    TextureVertexShaderChannel( const std::string & shaderSource );

};

typedef std::shared_ptr< TextureVertexShaderChannel >    TextureVertexShaderChannelPtr;

} // model
} // bv
