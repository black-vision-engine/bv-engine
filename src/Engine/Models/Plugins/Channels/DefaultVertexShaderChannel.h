#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"
#include "Engine/Interfaces/IValueSet.h"

namespace bv { namespace model {

class DefaultVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
protected:

    DefaultTexturesData *   m_texturesData;

public:

            DefaultVertexShaderChannel                      ( const std::string & shaderSource, IValueSetConstPtr valueSet );
    virtual ~DefaultVertexShaderChannel                     ();

    virtual const ITexturesData *       GetTexturesData     () const override;
    DefaultTexturesData *               GetTexturesDataImpl ();

    static  DefaultVertexShaderChannel * Create             ();
    static  DefaultVertexShaderChannel * Create             ( const std::string & shaderFile, IValueSetConstPtr values );

};

DEFINE_PTR_TYPE(DefaultVertexShaderChannel)

} //model
} //bv
