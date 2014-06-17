#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"


namespace bv { namespace model {

class DefaultPixelShaderChannel : public ShaderChannel< IPixelShaderChannel >
{
protected:

    RendererContext *       m_rendererContext;
    DefaultTexturesData *   m_texturesData;

public:

            DefaultPixelShaderChannel                       ( const std::string & shaderSource, IValueSetConstPtr valueSet, RendererContext * ctx = nullptr );
    virtual ~DefaultPixelShaderChannel                      ();

    virtual const RendererContext *     GetRendererContext  () const override;
    RendererContext *                   GetRendererContext  ();
    void                                SetRendererContext  ( RendererContext * ctx );

    virtual const ITexturesData *       GetTexturesData     () const override;
    DefaultTexturesData *               GetTexturesDataImpl ();

    static  DefaultPixelShaderChannel * Create              ( const std::string & shaderFile, IValueSetConstPtr values, RendererContext * ctx = nullptr );

};

DEFINE_PTR_TYPE(DefaultPixelShaderChannel)

} //model
} //bv
