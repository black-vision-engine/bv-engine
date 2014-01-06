#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv{ namespace model {

class TextureVertexShaderChannel : public model::ShaderChannel< model::IVertexShaderChannel >
{
public:

    TextureVertexShaderChannel( const std::string & shaderFile );

};

typedef std::shared_ptr< TextureVertexShaderChannel >    TextureVertexShaderChannelPtr;

} // model
} // bv
