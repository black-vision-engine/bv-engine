#pragma once

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"


namespace bv { namespace model {


class PixelShaderChannelBase : public ShaderChannel< IPixelShaderChannel >
{
protected:

    RendererContext *   m_rendererContext;

public:

            PixelShaderChannelBase                          ( const std::string & shaderFile, std::vector< IValue * > * values, RendererContext * ctx = nullptr );
    virtual ~PixelShaderChannelBase                         ();

    virtual const RendererContext *     GetRendererContext  () const;
    RendererContext *                   GetRendererContext  ();
    void                                SetRendererContext  ( RendererContext * ctx );

};

} //model
} //bv
