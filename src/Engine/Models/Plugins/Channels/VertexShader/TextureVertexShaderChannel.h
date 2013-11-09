#pragma once

#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugins/Parameter.h"


namespace bv{ namespace model {

class TextureVertexShaderChannelPD : public BaseParametersDescriptor
{
};

class TextureVertexShaderChannel : public model::ShaderChannel< model::IVertexShaderChannel, TextureVertexShaderChannelPD >
{
public:

    TextureVertexShaderChannel( const std::string& shaderFile );

};


} // model
} // bv