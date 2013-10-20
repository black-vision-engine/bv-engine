#pragma once

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugins/Parameter.h"

namespace bv { namespace model
{

class TexturePixelShaderChannelPD : public BaseParametersDescriptor
{
public:

    typedef std::vector< std::string > StringsVector;

    static const std::string            pluginName;

    static const StringsVector          alphaParamName;
    static const StringsVector          txMatrixParamName;

    explicit TexturePixelShaderChannelPD();
};

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string TexturePixelShaderChannelPD::pluginName( "SimpleTexturePlugin" );

class TexturePixelShaderChannel : public model::ShaderChannel< model::IPixelShaderChannel, TexturePixelShaderChannelPD >
{
    std::vector< model::ParamFloat * >      m_alphaParams;
    std::vector< model::ValueFloat * >      m_alphaValues;

    std::vector< model::ParamTransform * >  m_texTransformParams;
    std::vector< model::ValueMat4 * >       m_texTransformValues;

public:
    virtual void                    Update( float t );

    TexturePixelShaderChannel( const std::string& shaderFile, const std::vector< FloatInterpolator >& alphas, const std::vector< TransformF >& texTransforms );
};

} // model
} // bv
