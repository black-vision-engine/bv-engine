#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"


namespace bv { namespace model {

class DefaultVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
protected:

            DefaultVertexShaderChannel                       ( const std::string & shaderFile, const IValueSet * valueSet );

public:

    static  DefaultVertexShaderChannel * Create              ( const std::string & shaderFile, const IValueSet * values );

};

typedef std::shared_ptr< DefaultVertexShaderChannel > DefaultVertexShaderChannelPtr;

} //model
} //bv
