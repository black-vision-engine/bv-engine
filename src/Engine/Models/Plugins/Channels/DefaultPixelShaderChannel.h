#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"


namespace bv { namespace model {

class DefaultPixelShaderChannel : public ShaderChannel< IPixelShaderChannel >
{
protected:

    RendererContext *   m_rendererContext;

protected:

            DefaultPixelShaderChannel                       ( const std::string & shaderSource, const IValueSet * valueSet, RendererContext * ctx = nullptr );

public:

    virtual ~DefaultPixelShaderChannel                      ();

    virtual const RendererContext *     GetRendererContext  () const;
    RendererContext *                   GetRendererContext  ();
    void                                SetRendererContext  ( RendererContext * ctx );

    static  DefaultPixelShaderChannel * Create              ( const std::string & shaderFile, const IValueSet * values, RendererContext * ctx = nullptr );

};

typedef std::shared_ptr< DefaultPixelShaderChannel > DefaultPixelShaderChannelPtr;

} //model
} //bv
