#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Interfaces/IValueSet.h"

namespace bv { namespace model {

class DefaultVertexShaderChannel;
DEFINE_PTR_TYPE(DefaultVertexShaderChannel)

class DefaultVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
protected:

    DefaultTexturesDataPtr      m_texturesData;

public:

            DefaultVertexShaderChannel                          ( const std::string & shaderSource, IValueSetConstPtr valueSet );
    virtual ~DefaultVertexShaderChannel                         ();

    virtual ITexturesDataConstPtr           GetTexturesData     () const override;
    DefaultTexturesDataPtr                  GetTexturesDataImpl ();

    static  DefaultVertexShaderChannelPtr   Create              ();
    static  DefaultVertexShaderChannelPtr   Create              ( const std::string & shaderFile, IValueSetConstPtr values );
    static  DefaultVertexShaderChannelPtr   Create              ( IValueSetConstPtr values );

};

} //model
} //bv
