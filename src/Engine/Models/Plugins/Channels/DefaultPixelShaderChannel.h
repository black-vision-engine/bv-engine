#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

namespace bv { namespace model {

class DefaultPixelShaderChannel;
DEFINE_PTR_TYPE(DefaultPixelShaderChannel)

class DefaultPixelShaderChannel : public ShaderChannel< IPixelShaderChannel >
{
protected:

    RendererContextPtr      m_rendererContext;
    DefaultTexturesDataPtr  m_texturesData;

public:

            DefaultPixelShaderChannel                       ( const std::string & shaderSource, IValueSetConstPtr valueSet, RendererContextPtr ctx = nullptr );
    virtual ~DefaultPixelShaderChannel                      ();

    virtual RendererContextConstPtr         GetRendererContext  () const override;
    RendererContextPtr                      GetRendererContext  ();

    void                                    SetRendererContext  ( RendererContextPtr ctx );

    virtual ITexturesDataConstPtr           GetTexturesData     () const override;
    DefaultTexturesDataPtr                  GetTexturesDataImpl ();

    //FIXME: shitty hack
    void                                    OverrideTexturesData( DefaultTexturesDataPtr  texturesData );

    static  DefaultPixelShaderChannelPtr    Create              ( const std::string & shaderFile, IValueSetConstPtr values, RendererContextPtr ctx = nullptr );

};

} //model
} //bv
