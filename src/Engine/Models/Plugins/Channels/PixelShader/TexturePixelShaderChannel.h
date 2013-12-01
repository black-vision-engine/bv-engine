#pragma once

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameters/PluginParameters.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv { namespace model
{

class TexturePixelShaderChannelPD : public BaseParametersDescriptor
{
public:

    static const std::string    pluginName;

    static std::string          alphaParamName[];
    static std::string          txMatrixParamName[];

    TexturePixelShaderChannelPD()
        : BaseParametersDescriptor( pluginName )
    {}
};


class TexturePixelShaderChannel : public model::PixelShaderChannelBase< TexturePixelShaderChannelPD >
{
    std::vector< model::ParamFloat >        m_alphaParams;
    std::vector< model::ValueFloat * >      m_alphaValues;

    std::vector< model::ParamTransform >    m_texTransformParams;
    std::vector< model::ValueMat4 * >       m_texTransformValues;

public:

    virtual void                    Update( TimeType t );

    TexturePixelShaderChannel( const std::string & shaderFile, const std::vector< ParamFloat > & alphas, const std::vector< ParamTransform > & texTransforms );

};

} // model
} // bv
