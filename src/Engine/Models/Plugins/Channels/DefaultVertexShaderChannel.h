#pragma once

#include <memory>

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTexturesData.h"


namespace bv { namespace model {

class DefaultVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
protected:

    DefaultTexturesData *   m_texturesData;

public:

            DefaultVertexShaderChannel                      ( const std::string & shaderSource, const IValueSet * valueSet );
    virtual ~DefaultVertexShaderChannel                     ();

    virtual const ITexturesData *       GetTexturesData     () const override;
    DefaultTexturesData *               GetTexturesDataImpl ();

    static  DefaultVertexShaderChannel * Create             ();
    static  DefaultVertexShaderChannel * Create             ( const std::string & shaderFile, const IValueSet * values );

};

typedef std::shared_ptr< DefaultVertexShaderChannel > DefaultVertexShaderChannelPtr;

} //model
} //bv
