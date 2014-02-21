#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"


namespace bv { namespace model {

class DefaultVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
protected:

            DefaultVertexShaderChannel                       ( const std::string & shaderSource, const IValueSet * valueSet );

public:

    static  DefaultVertexShaderChannel * Create              ();

};

typedef std::shared_ptr< DefaultVertexShaderChannel > DefaultVertexShaderChannelPtr;

} //model
} //bv
